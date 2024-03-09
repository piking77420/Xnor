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
	m_ToneMapping.InitializeResources();
	m_SkyboxRenderer.InitializeResources();
	m_LightManager.InitResources();

	Rhi::PrepareUniform();
}

void Renderer::BeginFrame(const Scene& scene)
{
	m_LightManager.BeginFrame(scene);
	Rhi::ClearBuffer(static_cast<BufferFlag>(BufferFlagColorBit | BufferFlagDepthBit));
}

void Renderer::EndFrame(const Scene& scene)
{
	m_LightManager.EndFrame(scene);
}

void Renderer::RenderViewport(const Viewport& viewport,
	Scene& scene, Skybox& skybox) const
{
	BindCamera(*viewport.camera,viewport.viewPortSize);

	std::vector<const MeshRenderer*> meshrenderers;
	scene.GetAllComponentOfType<MeshRenderer>(&meshrenderers);

	const ViewportData& viewportData = viewport.viewportData;

	DefferedRendering(meshrenderers,viewportData,viewport.viewPortSize);
	ForwardPass(meshrenderers, skybox, viewport, viewport.viewPortSize, viewport.isEditor);
	
	const RenderPassBeginInfo renderPassBeginInfo =
	{
		.frameBuffer = viewport.frameBuffer,
		.renderAreaOffset = { 0, 0,},
		.renderAreaExtent = viewport.viewPortSize,
		.clearBufferFlags = static_cast<BufferFlag>(BufferFlagColorBit | BufferFlagDepthBit),
		.clearColor = clearColor
	};

	viewport.colorPass.BeginRenderPass(renderPassBeginInfo);
	m_ToneMapping.ComputeToneMaping(*viewport.viewportData.colorAttachment,m_Quad);
	viewport.colorPass.EndRenderPass();
}

void Renderer::SwapBuffers()
{
	Rhi::SwapBuffers();
}

void Renderer::DefferedRendering(const std::vector<const MeshRenderer*>& meshRenderers,const ViewportData& viewportData,const Vector2i viewportSize) const 
{
	const RenderPassBeginInfo renderPassBeginInfo =
	{
		.frameBuffer = viewportData.gframeBuffer,
		.renderAreaOffset = { 0, 0,},
		.renderAreaExtent = viewportSize,
		.clearBufferFlags = static_cast<BufferFlag>(BufferFlagColorBit | BufferFlagDepthBit),
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
		.clearBufferFlags = static_cast<BufferFlag>(BufferFlagColorBit),
		.clearColor = clearColor
	};

	Rhi::DepthTest(false);
	viewportData.colorPass.BeginRenderPass(renderPassBeginInfoLit);
	m_GBufferShaderLit->Use();
	// Set Shader Info
	viewportData.positionAtttachment->BindTexture(GbufferPosition);
	viewportData.normalAttachement->BindTexture(GbufferNormal);
	viewportData.albedoAttachment->BindTexture(GbufferAlbedo);
	Rhi::DrawQuad(m_Quad->GetId());
	m_GBufferShaderLit->Unuse();
	Rhi::DepthTest(true);

}

void Renderer::ForwardPass(const std::vector<const MeshRenderer*>& meshRenderers,Skybox& skybox, const Viewport& Viewport,
	Vector2i viewportSize, bool isEditor) const
{

	const ViewportData& viewportData = Viewport.viewportData;
	
	const RenderPassBeginInfo renderPassBeginInfoLit =
	{
		.frameBuffer = viewportData.renderBuffer,
		.renderAreaOffset = { 0, 0,},
		.renderAreaExtent = viewportSize,
		.clearBufferFlags = static_cast<BufferFlag>(BufferFlagNone),
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
		m_LightManager.DrawLightGizmo(*Viewport.camera);
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
		const Vector3&& center  = (modelAabb.max + modelAabb.min) * 0.5f;

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
		modelData.meshRenderIndex = i + 1;

		
		try
		{
			modelData.normalInvertMatrix = transform.worldMatrix.Inverted().Transposed();
		}
		catch (const std::invalid_argument&)
		{
			modelData.normalInvertMatrix = Matrix::Identity();
		}
		
		Rhi::UpdateModelUniform(modelData);

		if (meshRenderer->material.albedo.IsValid())
			meshRenderer->material.albedo->BindTexture(0);

		if (meshRenderer->material.normalMap.IsValid())
			meshRenderer->material.normalMap->BindTexture(1);

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
	m_GBufferShaderLit->SetInt("gPosition", GbufferPosition);
	m_GBufferShaderLit->SetInt("gNormal", GbufferNormal);
	m_GBufferShaderLit->SetInt("gAlbedoSpec", GbufferAlbedo);
	m_GBufferShaderLit->Unuse();
	
	m_GBufferShader = ResourceManager::Get<Shader>("gbuffer");
	m_GBufferShader->CreateInRhi();
	// Init diffuse Texture for gbuffer
	m_GBufferShader->Use();
	m_GBufferShader->SetInt("material.albedo", 0);
	m_GBufferShader->SetInt("material.normalMap", 1);
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

