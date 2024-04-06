#include "rendering/renderer.hpp"

#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"
#include "scene/component/mesh_renderer.hpp"

using namespace XnorCore;

void Renderer::Initialize()
{
	Rhi::SetClearColor(clearColor);

	InitResources();
	m_SkyboxRenderer.InitializeResources();
	m_LightManager.InitResources();
	m_PostProcessPass.Init();
	Rhi::PrepareRendering();
}

void Renderer::BeginFrame(const Scene& scene)
{
	m_LightManager.BeginFrame(scene, *this);
	Rhi::ClearBuffer(static_cast<BufferFlag::BufferFlag>(BufferFlag::ColorBit | BufferFlag::DepthBit));
}

void Renderer::EndFrame(const Scene& scene)
{
	m_LightManager.EndFrame(scene);
}

void Renderer::RenderViewport(const Viewport& viewport, const Scene& scene) const
{
	BindCamera(*viewport.camera,viewport.viewPortSize);

	std::vector<const MeshRenderer*> meshrenderers;
	scene.GetAllComponentOfType<MeshRenderer>(&meshrenderers);

	const ViewportData& viewportData = viewport.viewportData;

	DefferedRendering(meshrenderers, scene.skybox, viewportData, viewport.viewPortSize);
	ForwardPass(meshrenderers, scene.skybox, viewport, viewport.viewPortSize, viewport.isEditor);
	
	if (viewportData.usePostProcess)
		m_PostProcessPass.Compute(*viewport.viewportData.colorAttachment , *viewport.m_Image, viewportData.postprocessRendertarget);
}

void Renderer::RenderNonShaded(const Camera& camera,const RenderPassBeginInfo& renderPassBeginInfo, const RenderPass& renderPass,
	const Pointer<Shader>& shadertoUse, const Scene& scene,	const bool_t drawEditorUi
) const
{
	std::vector<const MeshRenderer*> meshrenderers;
	scene.GetAllComponentOfType<MeshRenderer>(&meshrenderers);
	BindCamera(camera, renderPassBeginInfo.renderAreaOffset + renderPassBeginInfo.renderAreaExtent);
	
	renderPass.BeginRenderPass(renderPassBeginInfo);
	DrawAllMeshRendersNonShaded(meshrenderers, scene);
	
	if (drawEditorUi)
	{
		m_LightManager.DrawLightGizmoWithShader(camera, scene, shadertoUse);
	}
	
	renderPass.EndRenderPass();
}

void Renderer::SwapBuffers() const
{
	Rhi::SwapBuffers();
}

void Renderer::DefferedRendering(const std::vector<const MeshRenderer*>& meshRenderers, const Skybox& skybox, const ViewportData& viewportData, const Vector2i viewportSize) const 
{
	const RenderPassBeginInfo renderPassBeginInfo =
	{
		.frameBuffer = viewportData.gFramebuffer,
		.renderAreaOffset = { 0, 0 },
		.renderAreaExtent = viewportSize,
		.clearBufferFlags = static_cast<BufferFlag::BufferFlag>(BufferFlag::ColorBit | BufferFlag::DepthBit),
		.clearColor = clearColor
	};
	
	viewportData.gBufferPass.BeginRenderPass(renderPassBeginInfo);
	m_GBufferShader->Use();
	DrawMeshRendersByType(meshRenderers, MaterialType::Opaque);
	m_GBufferShader->Unuse();
	viewportData.gBufferPass.EndRenderPass();

	const RenderPassBeginInfo renderPassBeginInfoLit =
	{
		.frameBuffer = viewportData.renderBuffer,
		.renderAreaOffset = { 0, 0 },
		.renderAreaExtent = viewportSize,
		.clearBufferFlags = BufferFlag::ColorBit,
		.clearColor = clearColor
	};

	
	viewportData.colorPass.BeginRenderPass(renderPassBeginInfoLit);
	m_GBufferShaderLit->Use();
	
	// Set G buffer Shader Info
	viewportData.BindDescriptor();
	skybox.BindDesriptorSet();
	m_LightManager.BindShadowMap();
	
	Rhi::DrawModel(m_Quad->GetId());
	
	skybox.UnbindDesriptorSet();
	viewportData.UnbindDescriptor();
	viewportData.colorPass.EndRenderPass();
	
	m_GBufferShaderLit->Unuse();

}

void Renderer::ForwardPass(const std::vector<const MeshRenderer*>& meshRenderers, const Skybox& skybox, const Viewport& viewport, const Vector2i viewportSize, const bool_t isEditor) const
{
	const ViewportData& viewportData = viewport.viewportData;
	
	const RenderPassBeginInfo renderPassBeginInfoLit =
	{
		.frameBuffer = viewportData.renderBuffer,
		.renderAreaOffset = { 0, 0 },
		.renderAreaExtent = viewportSize,
		.clearBufferFlags = BufferFlag::None,
		.clearColor = clearColor
	};

	viewportData.colorPass.BeginRenderPass(renderPassBeginInfoLit);
	
	m_Forward->Use();
	DrawMeshRendersByType(meshRenderers, MaterialType::Lit);
	m_Forward->Unuse();

	if (isEditor)
	{
		DrawAabb(meshRenderers);
	}

	m_SkyboxRenderer.DrawSkymap(m_Cube,skybox);

	if (isEditor)
	{
		m_LightManager.DrawLightGizmo(*viewport.camera, *World::scene);
	}

	viewportData.colorPass.EndRenderPass();
}

void Renderer::DrawAabb(const std::vector<const MeshRenderer*>& meshRenderers) const
{
	m_GizmoShader->Use();
	Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Line);
	ModelUniformData modelData;

	for (const MeshRenderer* const meshRenderer : meshRenderers)
	{
		if (!meshRenderer->model.IsValid())
			continue;

		if (!meshRenderer->drawModelAabb)
			continue;

		const Transform& transform =  meshRenderer->GetEntity()->transform;
		const Bound&& modelAabb = meshRenderer->model->GetAabb().GetAabbFromTransform(transform);
		const Matrix&& trsAabb = Matrix::Trs(modelAabb.center, Quaternion::Identity(), modelAabb.size * 0.5f);
		modelData.model = trsAabb;
		Rhi::UpdateModelUniform(modelData);

		Rhi::DrawModel(m_Cube->GetId());
	}
	
	m_GizmoShader->Unuse();
	Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);
}


void Renderer::DrawMeshRendersByType(const std::vector<const MeshRenderer*>& meshRenderers, const MaterialType materialType) const
{
	Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);

	for (const MeshRenderer* const meshRenderer : meshRenderers)
	{
		if (meshRenderer->material.materialType != materialType)
			continue;
		
		const Transform& transform = meshRenderer->GetEntity()->transform;
		ModelUniformData modelData;
		modelData.model = transform.worldMatrix;
		modelData.meshRenderIndex = reinterpret_cast<uint64_t>(meshRenderer->GetEntity());
		
		try
		{
			modelData.normalInvertMatrix = transform.worldMatrix.Inverted().Transposed();
		}
		catch (const std::invalid_argument&)
		{
			modelData.normalInvertMatrix = Matrix::Identity();
		}
		
		Rhi::UpdateModelUniform(modelData);

		if (meshRenderer->material.albedoTexture.IsValid())
			meshRenderer->material.albedoTexture->BindTexture(MaterialTextureEnum::Albedo);
		
		if (meshRenderer->material.metallicTexture.IsValid())
			meshRenderer->material.metallicTexture->BindTexture(MaterialTextureEnum::Metallic);

		if (meshRenderer->material.roughnessTexture.IsValid())
			meshRenderer->material.roughnessTexture->BindTexture(MaterialTextureEnum::Roughness);

		if (meshRenderer->material.normalTexture.IsValid())
			meshRenderer->material.normalTexture->BindTexture(MaterialTextureEnum::Normal);

		if (meshRenderer->material.ambientOcclusionTexture.IsValid())
			meshRenderer->material.ambientOcclusionTexture->BindTexture(MaterialTextureEnum::AmbiantOcclusion);

		if (meshRenderer->model.IsValid())
		{
			Rhi::BindMaterial(meshRenderer->material);
			Rhi::DrawModel(meshRenderer->model->GetId());
		}
	}
}

void Renderer::DrawAllMeshRenders(const std::vector<const MeshRenderer*>& meshRenderers, const Scene& scene) const
{
	Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);

	for (const MeshRenderer* const meshRenderer : meshRenderers)
	{
		const Transform& transform = meshRenderer->GetEntity()->transform;
		ModelUniformData modelData;
		modelData.model = transform.worldMatrix;
		// +1 to avoid the black color of the attachment be a valid index  
		modelData.meshRenderIndex = scene.GetEntityIndex(meshRenderer->GetEntity()) + 1;
		
		try
		{
			modelData.normalInvertMatrix = transform.worldMatrix.Inverted().Transposed();
		}
		catch (const std::invalid_argument&)
		{
			modelData.normalInvertMatrix = Matrix::Identity();
		}
		
		Rhi::UpdateModelUniform(modelData);

		if (meshRenderer->material.albedoTexture.IsValid())
			meshRenderer->material.albedoTexture->BindTexture(MaterialTextureEnum::Albedo);
		
		if (meshRenderer->material.metallicTexture.IsValid())
			meshRenderer->material.metallicTexture->BindTexture(MaterialTextureEnum::Metallic);

		if (meshRenderer->material.roughnessTexture.IsValid())
			meshRenderer->material.roughnessTexture->BindTexture(MaterialTextureEnum::Roughness);

		if (meshRenderer->material.normalTexture.IsValid())
			meshRenderer->material.normalTexture->BindTexture(MaterialTextureEnum::Normal);

		if (meshRenderer->material.ambientOcclusionTexture.IsValid())
			meshRenderer->material.ambientOcclusionTexture->BindTexture(MaterialTextureEnum::AmbiantOcclusion);
		
		if (meshRenderer->model.IsValid())
		{
			Rhi::BindMaterial(meshRenderer->material);
			Rhi::DrawModel(meshRenderer->model->GetId());
		}
	}
}

void Renderer::DrawAllMeshRendersNonShaded(const std::vector<const MeshRenderer*>& meshRenderers, const Scene& scene) const
{
	Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);

	for (const MeshRenderer* const meshRenderer : meshRenderers)
	{
		const Transform& transform = meshRenderer->GetEntity()->transform;
		ModelUniformData modelData;
		modelData.model = transform.worldMatrix;
		// +1 to avoid the black color of the attachment be a valid index  
		modelData.meshRenderIndex = scene.GetEntityIndex(meshRenderer->GetEntity()) + 1;
		
		try
		{
			modelData.normalInvertMatrix = transform.worldMatrix.Inverted().Transposed();
		}
		catch (const std::invalid_argument&)
		{
			modelData.normalInvertMatrix = Matrix::Identity();
		}
		
		Rhi::UpdateModelUniform(modelData);
		
		if (meshRenderer->model.IsValid())
		{
			Rhi::BindMaterial(meshRenderer->material);
			Rhi::DrawModel(meshRenderer->model->GetId());
		}
	}
}

void Renderer::BindCamera(const Camera& camera,const Vector2i screenSize) const
{
	CameraUniformData cam;
	camera.GetView(&cam.view);
	camera.GetProjection(screenSize, &cam.projection);
	cam.cameraPos = camera.position;
	cam.near = camera.near;
	cam.far = camera.far;
	Rhi::UpdateCameraUniform(cam);
}

void Renderer::InitResources()
{
	// Deferred 
	m_GBufferShaderLit = ResourceManager::Get<Shader>("deferred_opaque");
	m_GBufferShaderLit->SetDepthFunction( DepthFunction::DepthFunction::Disable);
	
	m_GBufferShaderLit->CreateInRhi();
	m_GBufferShaderLit->Use();
	
	m_GBufferShaderLit->SetInt("gPosition", DefferedDescriptor::Position);
	m_GBufferShaderLit->SetInt("gNormal", DefferedDescriptor::Normal);
	m_GBufferShaderLit->SetInt("gAlbedoSpec", DefferedDescriptor::Albedo);
	m_GBufferShaderLit->SetInt("gMetallicRoughessReflectance", DefferedDescriptor::MetallicRoughessReflectance);
	m_GBufferShaderLit->SetInt("gAmbiantOcclusion", DefferedDescriptor::AmbiantOcclusion);
	m_GBufferShaderLit->SetInt("gEmissive", DefferedDescriptor::Emissivive);

	m_GBufferShaderLit->SetInt("irradianceMap", DefferedDescriptor::SkyboxIrradiance);
	m_GBufferShaderLit->SetInt("prefilterMap", DefferedDescriptor::SkyboxPrefilterMap);
	m_GBufferShaderLit->SetInt("brdfLUT", DefferedDescriptor::SkyboxPrecomputeBrdf);
	
	m_GBufferShaderLit->SetInt("dirLightShadowMap", ShadowTextureBinding::Directional);
	m_GBufferShaderLit->SetInt("spotLightShadowArray", ShadowTextureBinding::SpotLight);
	m_GBufferShaderLit->SetInt("pointLightCubemapArrayPixelDistance", ShadowTextureBinding::PointLightCubemapArrayPixelDistance);

	m_GBufferShaderLit->Unuse();
	
	m_GBufferShader = ResourceManager::Get<Shader>("gbuffer");

	constexpr ShaderProgramCullInfo cullInfo =
	{
		.enableCullFace = true,
		.cullFace = CullFace::Front,
		.frontFace = FrontFace::CCW
	};

	m_GBufferShader->SetFaceCullingInfo(cullInfo);
	m_GBufferShader->CreateInRhi();

	// Init diffuse Texture for gbuffer
	m_GBufferShader->Use();
	m_GBufferShader->SetInt("material.albedoMap", MaterialTextureEnum::Albedo);
	m_GBufferShader->SetInt("material.metallicMap", MaterialTextureEnum::Metallic);
	m_GBufferShader->SetInt("material.roughnessMap", MaterialTextureEnum::Roughness);
	m_GBufferShader->SetInt("material.normalMap", MaterialTextureEnum::Normal);
	m_GBufferShader->SetInt("material.ambiantOcclusionMap", MaterialTextureEnum::AmbiantOcclusion);
	m_GBufferShader->Unuse();
	// End deferred

	// Forward
	m_Forward = ResourceManager::Get<Shader>("basic_shader");
	m_Forward->CreateInRhi();

	m_DrawTextureToScreenShader = ResourceManager::Get<Shader>("draw_texture_to_screen");
	m_DrawTextureToScreenShader->CreateInRhi();
	
	m_GizmoShader = ResourceManager::Get<Shader>("gizmo_shader");
	m_GizmoShader->CreateInRhi();

	m_DrawTextureToScreenShader->Use();
	m_DrawTextureToScreenShader->SetInt("bufferTextureId", 0);
	m_DrawTextureToScreenShader->Unuse();
	// End forward

	// Primitive
	m_Cube = ResourceManager::Get<Model>("assets/models/cube.obj");
	m_Quad = ResourceManager::Get<Model>("assets/models/quad.obj");
}
