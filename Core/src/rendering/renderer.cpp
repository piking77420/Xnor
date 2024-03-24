#include "rendering/renderer.hpp"

#include "rendering/rhi.hpp"
#include "rendering/light/directional_light.hpp"
#include "rendering/light/point_light.hpp"
#include "rendering/light/spot_light.hpp"
#include "resource/resource_manager.hpp"
#include "scene/component/mesh_renderer.hpp"

using namespace XnorCore;

void Renderer::Initialize()
{
	Rhi::SetClearColor(clearColor);

	InitResources();
	m_SkyboxRenderer.InitializeResources();
	m_LightManager.InitResources();

	Rhi::PrepareUniform();
}

void Renderer::BeginFrame(const Scene& scene)
{
	m_LightManager.BeginFrame(scene);
	Rhi::ClearBuffer(static_cast<BufferFlag::BufferFlag>(BufferFlag::ColorBit | BufferFlag::DepthBit));
}

void Renderer::EndFrame(const Scene& scene)
{
	m_LightManager.EndFrame(scene);
}

void Renderer::RenderViewport(const Viewport& viewport,
	Scene& scene) const
{
	BindCamera(*viewport.camera,viewport.viewPortSize);

	std::vector<const MeshRenderer*> meshrenderers;
	scene.GetAllComponentOfType<MeshRenderer>(&meshrenderers);

	const ViewportData& viewportData = viewport.viewportData;

	DefferedRendering(meshrenderers,scene.skybox,viewportData,viewport.viewPortSize);
	ForwardPass(meshrenderers, scene.skybox, viewport, viewport.viewPortSize, viewport.isEditor);
	
	if (viewportData.usePostProcess)
	m_PostProcessPass.Compute(*viewport.viewportData.colorAttachment , *viewport.GetImage(), viewportData.postprocessRendertarget);
}

void Renderer::RenderNonShaded(const Camera& camera,const RenderPassBeginInfo& renderPassBeginInfo, const RenderPass& renderPass,
	const Pointer<Shader>& shadertoUse, const Scene& scene,	const bool_t drawEditorUi
) const
{
	std::vector<const MeshRenderer*> meshrenderers;
	scene.GetAllComponentOfType<MeshRenderer>(&meshrenderers);
	
	renderPass.BeginRenderPass(renderPassBeginInfo);
	shadertoUse->Use();
	DrawAllMeshRenders(meshrenderers, scene);
	if (drawEditorUi)
	{
		m_LightManager.DrawLightGizmoWithShader(camera, scene, shadertoUse);
	}
	shadertoUse->Unuse();
	renderPass.EndRenderPass();
}

void Renderer::SwapBuffers()
{
	Rhi::SwapBuffers();
}

void Renderer::DefferedRendering(const std::vector<const MeshRenderer*>& meshRenderers,
	const Skybox& skybox,const ViewportData& viewportData,const Vector2i viewportSize) const 
{
	const RenderPassBeginInfo renderPassBeginInfo =
	{
		.frameBuffer = viewportData.gframeBuffer,
		.renderAreaOffset = { 0, 0,},
		.renderAreaExtent = viewportSize,
		.clearBufferFlags = static_cast<BufferFlag::BufferFlag>(BufferFlag::ColorBit | BufferFlag::DepthBit),
		.clearColor = clearColor
	};
	
	viewportData.gbufferPass.BeginRenderPass(renderPassBeginInfo);
	m_GBufferShader->Use();
	DrawMeshRendersByType(meshRenderers, MaterialType::Opaque);
	m_GBufferShader->Unuse();
	viewportData.gbufferPass.EndRenderPass();

	const RenderPassBeginInfo renderPassBeginInfoLit =
	{
		.frameBuffer = viewportData.renderBuffer,
		.renderAreaOffset = { 0, 0 },
		.renderAreaExtent = viewportSize,
		.clearBufferFlags = BufferFlag::ColorBit,
		.clearColor = clearColor
	};

	Rhi::DepthTest(false);
	viewportData.colorPass.BeginRenderPass(renderPassBeginInfoLit);
	m_GBufferShaderLit->Use();
	
	// Set G buffer Shader Info
	viewportData.positionAtttachment->BindTexture(Gbuffer::Position);
	viewportData.normalAttachement->BindTexture(Gbuffer::Normal);
	viewportData.albedoAttachment->BindTexture(Gbuffer::Albedo);
	viewportData.metallicRougnessReflectance->BindTexture(Gbuffer::MetallicRoughessReflectance);
	viewportData.emissiveAmbiantOcclusion->BindTexture(Gbuffer::EmissiveAmbiantOcclusion);
	
	skybox.irradianceMap->BindTexture(10);
	skybox.prefilterMap->BindTexture(11);
	skybox.precomputeBrdfTexture->BindTexture(12);

	Rhi::DrawModel(m_Quad->GetId());
	m_GBufferShaderLit->Unuse();
	Rhi::DepthTest(true);
	
	viewportData.colorPass.EndRenderPass();
}

void Renderer::ForwardPass(const std::vector<const MeshRenderer*>& meshRenderers, const Skybox& skybox, const Viewport& viewport, const Vector2i viewportSize, const bool_t isEditor) const
{
	const ViewportData& viewportData = viewport.viewportData;
	
	const RenderPassBeginInfo renderPassBeginInfoLit =
	{
		.frameBuffer = viewportData.renderBuffer,
		.renderAreaOffset = { 0, 0,},
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

	for (uint32_t i = 0; i < meshRenderers.size(); i++)
	{
		const MeshRenderer* meshRenderer = meshRenderers[i];

		if (!meshRenderer->model.IsValid())
			continue;

		if (!meshRenderer->drawModelAabb)
			continue;

		const Transform& transform =  meshRenderer->entity->transform;
		const Model::Aabb&& modelAabb = meshRenderer->model->GetAabb();

		const Vector3&& aabbSize = (modelAabb.max - modelAabb.min) * 0.5f;
		const Vector3&& center = (modelAabb.max + modelAabb.min) * 0.5f;

		const Matrix&& trsAabb = Matrix::Trs(center, Quaternion::Identity(), aabbSize);
		modelData.model = transform.worldMatrix * trsAabb;
		Rhi::UpdateModelUniform(modelData);

		Rhi::DrawModel(m_Cube->GetId());
	}
	
	m_GizmoShader->Unuse();
	Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);
}

void Renderer::DrawMeshRendersByType(const std::vector<const MeshRenderer*>& meshRenderers, const MaterialType materialType) const
{
	Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);

	for (uint32_t i = 0; i < meshRenderers.size(); i++)
	{
		const MeshRenderer* meshRenderer =  meshRenderers[i];
		
		if (meshRenderer->material.materialType != materialType)
			continue;
		
		Transform& transform = meshRenderer->entity->transform;
		ModelUniformData modelData;
		modelData.model = transform.worldMatrix;
		modelData.meshRenderIndex = reinterpret_cast<uint64_t>(meshRenderers[i]->entity);
		
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

		if (meshRenderer->material.ambiantOcclusionTexture.IsValid())
			meshRenderer->material.ambiantOcclusionTexture->BindTexture(MaterialTextureEnum::AmbiantOcclusion);

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

	for (uint32_t i = 0; i < meshRenderers.size(); i++)
	{
		const MeshRenderer* meshRenderer =  meshRenderers[i];
		
		Transform& transform = meshRenderer->entity->transform;
		ModelUniformData modelData;
		modelData.model = transform.worldMatrix;
		// +1 to avoid the black color of the attachment be a valid index  
		modelData.meshRenderIndex = scene.GetEntityIndex(meshRenderer->entity) + 1;
		
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

		if (meshRenderer->material.ambiantOcclusionTexture.IsValid())
			meshRenderer->material.ambiantOcclusionTexture->BindTexture(MaterialTextureEnum::AmbiantOcclusion);
		
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
	Rhi::UpdateCameraUniform(cam);
}

void Renderer::InitResources()
{
	// Deferred 
	m_GBufferShaderLit = ResourceManager::Get<Shader>("deferred_opaque");
	m_GBufferShaderLit->CreateInRhi();
	m_GBufferShaderLit->Use();
	
	m_GBufferShaderLit->SetInt("gPosition", Gbuffer::Position);
	m_GBufferShaderLit->SetInt("gNormal", Gbuffer::Normal);
	m_GBufferShaderLit->SetInt("gAlbedoSpec", Gbuffer::Albedo);
	m_GBufferShaderLit->SetInt("gMetallicRoughessReflectance", Gbuffer::MetallicRoughessReflectance);
	m_GBufferShaderLit->SetInt("gEmissiveAmbiantOcclusion", Gbuffer::EmissiveAmbiantOcclusion);

	m_GBufferShaderLit->SetInt("irradianceMap", 10);
	m_GBufferShaderLit->SetInt("prefilterMap", 11);
	m_GBufferShaderLit->SetInt("brdfLUT", 12);
	m_GBufferShaderLit->Unuse();
	
	m_GBufferShader = ResourceManager::Get<Shader>("gbuffer");
	m_GBufferShader->SetFaceCullingInfo({true,CullFace::Front,FrontFace::CCW});
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

