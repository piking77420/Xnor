#include "rendering/renderer.hpp"

#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"
#include "scene/component/mesh_renderer.hpp"

using namespace XnorCore;

void Renderer::RenderMenu()
{
	if (ImGui::BeginMenu("CurrentScene"))
	{
		if (ImGui::Checkbox("DrawScene Octree",&m_RenderOctree.draw))
		{
			
		}
			
		ImGui::EndMenu();
	}
	
	if (m_RenderOctree.draw)
	{
		m_RenderOctree.Draw();
	}
}

void Renderer::Initialize()
{
	Rhi::SetClearColor(clearColor);

	InitResources();
	m_SkyboxRenderer.InitializeResources();
	m_LightManager.InitResources();
	m_PostProcessPass.Init();
	m_AnimationRender.InitResources();
	Rhi::PrepareRendering();
}

void Renderer::BeginFrame(const Scene& scene)
{
	m_LightManager.BeginFrame(scene, *this);
	m_AnimationRender.BeginFrame(scene, *this);
	Rhi::ClearBuffer(static_cast<BufferFlag::BufferFlag>(BufferFlag::ColorBit | BufferFlag::DepthBit));
	scene.GetAllComponentOfType<MeshRenderer>(&m_MeshRenderers);
	
	PrepareOctree();
}

void Renderer::EndFrame(const Scene& scene)
{
	m_LightManager.EndFrame(scene);
	m_AnimationRender.EndFrame();
}

void Renderer::RenderViewport(const Viewport& viewport, const Scene& scene) const
{
	BindCamera(*viewport.camera, viewport.viewPortSize);
	m_Frustum.UpdateFromCamera(*viewport.camera,viewport.GetAspect());
	const ViewportData& viewportData = viewport.viewportData;
	DeferredRendering(*viewport.camera, m_MeshRenderers, scene.skybox, viewportData, viewport.viewPortSize);
	ForwardPass(m_MeshRenderers, scene.skybox, viewport, viewport.viewPortSize, viewport.isEditor);
	
	if (viewportData.usePostProcess)
		m_PostProcessPass.Compute(*viewport.viewportData.colorAttachment , *viewport.image, viewportData.postprocessRendertarget);
}

void Renderer::RenderNonShaded(const Camera& camera,const RenderPassBeginInfo& renderPassBeginInfo, const RenderPass& renderPass,
	const Pointer<Shader>& shaderToUse, const Scene& scene,	const bool_t drawEditorUi
) const
{
	const Vector2i viewportSize = renderPassBeginInfo.renderAreaOffset + renderPassBeginInfo.renderAreaExtent;
	std::vector<const MeshRenderer*> meshrenderers;
	scene.GetAllComponentOfType<MeshRenderer>(&meshrenderers);
	BindCamera(camera,viewportSize );
	m_Frustum.UpdateFromCamera(camera,static_cast<float_t>(viewportSize.x) / static_cast<float_t>(viewportSize.y));
	renderPass.BeginRenderPass(renderPassBeginInfo);
	DrawAllMeshRendersNonShaded(meshrenderers, scene);
	
	if (drawEditorUi)
	{
		m_LightManager.DrawLightGizmoWithShader(camera, scene, shaderToUse);
	}
	
	renderPass.EndRenderPass();
}

void Renderer::SwapBuffers() const
{
	Rhi::SwapBuffers();
}

void Renderer::DeferredRendering(const Camera&, const std::vector<const MeshRenderer*>& meshRenderers, const Skybox& skybox, const ViewportData& viewportData, const Vector2i viewportSize) const 
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
	
	// Draw Simple Mesh
	m_GBufferShader->Use();
	DrawMeshRendersByType(meshRenderers, MaterialType::Opaque);
	m_GBufferShader->Unuse();
	// DrawSkinnedMesh
	m_AnimationRender.RenderAnimation();
	
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
	
	Rhi::DrawModel(DrawMode::Triangles, m_Quad->GetId());
	
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
	m_GizmoShader->SetVec3("color",{0.f,1.f,0.f});

	for (const MeshRenderer* const meshRenderer : meshRenderers)
	{
		if (!meshRenderer->mesh.IsValid())
			continue;

		if (!meshRenderer->drawModelAabb)
			continue;

		for (size_t i = 0; i < meshRenderer->mesh->models.GetSize(); i++)
		{
			const Pointer<Model>& model = meshRenderer->mesh->models[i];
			const Transform& transform =  meshRenderer->GetEntity()->transform;
			const Bound&& modelAabb = Bound::GetAabbFromTransform( model->GetAabb(), transform);
			const Matrix&& trsAabb = Matrix::Trs(modelAabb.center, Quaternion::Identity(), modelAabb.extents);
			modelData.model = trsAabb;
			Rhi::UpdateModelUniform(modelData);

			Rhi::DrawModel(DrawMode::Triangles, m_Cube->GetId());
		}
		
		
	}
	
	m_GizmoShader->Unuse();
	Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);
}

void Renderer::PrepareOctree() const
{
	std::vector<ObjectBounding<const MeshRenderer>> meshrenderWithAabb;
	
	for (uint32_t i = 0; i < World::scene->GetEntities().GetSize();i++)
	{
		Entity& ent = *World::scene->GetEntities()[i];

		const MeshRenderer* meshRenderer = nullptr;
		if (ent.TryGetComponent(&meshRenderer))
		{
			if (!meshRenderer->mesh.IsValid())
				continue;

			Bound bound;
			meshRenderer->GetAABB(&bound);

			ObjectBounding<const MeshRenderer> data;
			data.bound = bound;
			data.handle = meshRenderer;
			meshrenderWithAabb.emplace_back(data);
		}
	}
	m_RenderOctree.Update(meshrenderWithAabb);
}


void Renderer::DrawMeshRendersByType(const std::vector<const MeshRenderer*>& meshRenderers, const MaterialType materialType) const
{
	Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);

	for (const MeshRenderer* const meshRenderer : meshRenderers)
	{
		if (meshRenderer->material.materialType != materialType)
			continue;
		
		if (!meshRenderer->mesh.IsValid())
			continue;
		
		
		for (size_t i = 0; i < meshRenderer->mesh->models.GetSize(); i++)
		{
			const Pointer<Model>& model = meshRenderer->mesh->models[i];
			Bound aabb;
			meshRenderer->GetAABB(&aabb);
			if (m_Frustum.IsOnFrustum(aabb))
			{
				continue;
			}
		
		
			const Transform& transform = meshRenderer->GetEntity()->transform;
			ModelUniformData modelData;
			modelData.model = transform.worldMatrix;
		
			try
			{
				modelData.normalInvertMatrix = transform.worldMatrix.Inverted().Transposed();
			}
			catch (const std::invalid_argument&)
			{
				modelData.normalInvertMatrix = Matrix::Identity();
			}
		
			Rhi::UpdateModelUniform(modelData);

			if (model.IsValid())
			{
				meshRenderer->material.BindMaterial();
				Rhi::DrawModel(DrawMode::Triangles, model->GetId());
			}
		}
		
	}
}



void Renderer::DrawAllMeshRendersNonShaded(const std::vector<const MeshRenderer*>& meshRenderers, const Scene& scene) const
{
	Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);
	for (const MeshRenderer* const meshRenderer : meshRenderers)
	{
		for (size_t i = 0; i < meshRenderer->mesh->models.GetSize(); i++)
		{
			const Pointer<Model>& model = meshRenderer->mesh->models[i];
			
			Bound aabb;
			meshRenderer->GetAABB(&aabb);
			if (m_Frustum.IsOnFrustum(aabb))
			{
				continue;
			}
		
		
			const Transform& transform = meshRenderer->GetEntity()->transform;
			ModelUniformData modelData;
			modelData.model = transform.worldMatrix;
			// +1 to not let 0 index be a correct value in the frame buffer
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

			if (model.IsValid())
			{
				Rhi::DrawModel(DrawMode::Triangles, model->GetId());
			}
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
	m_GBufferShaderLit->SetDepthFunction(DepthFunction::Disable);
	
	m_GBufferShaderLit->CreateInInterface();
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
	m_GBufferShader->CreateInInterface();

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
	m_Forward->CreateInInterface();

	m_DrawTextureToScreenShader = ResourceManager::Get<Shader>("draw_texture_to_screen");
	m_DrawTextureToScreenShader->CreateInInterface();
	
	m_GizmoShader = ResourceManager::Get<Shader>("gizmo_shader");
	m_GizmoShader->CreateInInterface();

	m_DrawTextureToScreenShader->Use();
	m_DrawTextureToScreenShader->SetInt("bufferTextureId", 0);
	m_DrawTextureToScreenShader->Unuse();
	// End forward

	// Primitive
	m_Cube = ResourceManager::Get<Model>("assets/models/cube.obj");
	m_Quad = ResourceManager::Get<Model>("assets/models/quad.obj");
}
