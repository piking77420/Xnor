#include "rendering/renderer.hpp"


#include "rendering/light/point_light.hpp"
#include "rendering\light\directional_light.hpp"
#include "rendering\light\spot_light.hpp"
#include "resource/resource_manager.hpp"
#include "scene/component/mesh_renderer.hpp"

using namespace XnorCore;

Renderer::Renderer()
	: clearColor(0.f)
{
}

void Renderer::Initialize()
{
	Rhi::SetClearColor(clearColor);

	InitResources();
	m_ToneMapping.InitializeResources();
	m_SkyboxRenderer.InitializeResources();
	m_LightCuller.InitResources();

	Rhi::PrepareUniform();
}

void Renderer::Shutdown()
{
	DestroyAttachment();
}

void Renderer::RenderScene(const RendererContext& rendererContext) const
{
	const Scene& scene = World::scene;
	
	std::vector<const MeshRenderer*> meshrenderers;
	scene.GetAllComponentOfType<MeshRenderer>(&meshrenderers);
	
	std::vector<const PointLight*> pointLights;
	std::vector<const SpotLight*> spotLights;
	std::vector<const DirectionalLight*> directionalLights;
	scene.GetAllComponentOfType<PointLight>(&pointLights);
	scene.GetAllComponentOfType<SpotLight>(&spotLights);
	scene.GetAllComponentOfType<DirectionalLight>(&directionalLights);

	// Update Light
	m_LightCuller.UpdateLight(pointLights,spotLights,directionalLights);
	
	// Update Camera
	CameraUniformData cam;
	rendererContext.camera->GetView(&cam.view);
	rendererContext.camera->GetProjection(rendererContext.framebuffer->GetSize(), &cam.projection);
	cam.cameraPos = rendererContext.camera->position;
	Rhi::UpdateCameraUniform(cam);
	
	Rhi::SetViewport(m_RenderBuffer->GetSize());
	
	// Clear MainWindow // 
	Rhi::ClearColorAndDepth();
	
	DefferedRendering(meshrenderers, &rendererContext);
	// Blit depth of gbuffer to forward Pass
	Rhi::BlitFrameBuffer(m_GframeBuffer->GetId(), m_RenderBuffer->GetId(),
		{0, 0},m_GframeBuffer->GetSize(),
		{0, 0},m_RenderBuffer->GetSize(), Attachment::Depth, TextureFiltering::Nearest);
	
	// ForwardPass //
	ForwardRendering(meshrenderers, &rendererContext);
	m_SkyboxRenderer.DrawSkymap(m_Cube, World::skybox);
	if (rendererContext.isEditor)
	{
		m_LightCuller.DrawLightGizmo(pointLights, spotLights, directionalLights, *rendererContext.camera);
		
	}
	
	m_RenderBuffer->UnBindFrameBuffer();
	
	// DRAW THE FINAL IMAGE TEXTURE
	m_ToneMapping.ComputeToneMaping(*m_ColorAttachment, m_Quad);

	
	if (rendererContext.framebuffer != nullptr)
	{
		rendererContext.framebuffer->BindFrameBuffer();
		Rhi::ClearColorAndDepth();
		Rhi::SetViewport(rendererContext.framebuffer->GetSize());
	}
	
	m_DrawTextureToScreenShader->Use();
	m_ToneMapping.GetToneMapedImage().BindTexture(0);
	Rhi::DrawQuad(m_Quad->GetId());
	m_DrawTextureToScreenShader->Unuse();
	
	if (rendererContext.framebuffer != nullptr)
	{
		rendererContext.framebuffer->UnBindFrameBuffer();
	}
}

void Renderer::CompileShader()
{
	m_ToneMapping.RecompileShader();
}

void Renderer::SwapBuffers()
{
	Rhi::SwapBuffers();
}

void Renderer::OnResize(const vec2i windowSize)
{
	DestroyAttachment();
	m_ToneMapping.OnResizeWindow(windowSize);
	InitDefferedRenderingAttachment(windowSize);
	InitForwardRenderingAttachment(windowSize);
}

void Renderer::PrepareRendering(const vec2i windowSize)
{
	InitDefferedRenderingAttachment(windowSize);
	InitForwardRenderingAttachment(windowSize);
	m_ToneMapping.Prepare(windowSize);
}


void Renderer::DrawMeshRendersByType(const std::vector<const MeshRenderer*>& meshRenderers, const MaterialType materialtype) const
{
	Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);

	for (const MeshRenderer* meshRenderer : meshRenderers)
	{
		if (meshRenderer->material.materialType != materialtype)
			continue;
		
		Transform& transform = meshRenderer->entity->transform;
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


void Renderer::InitDefferedRenderingAttachment(const Vector2i windowSize)
{
	m_GframeBuffer = new FrameBuffer(windowSize);

	m_PositionAtttachment = new Texture(TextureInternalFormat::Rgb16F, windowSize);
	m_NormalAttachement = new Texture(TextureInternalFormat::Rgb16F, windowSize);
	m_AlbedoAttachment = new Texture(TextureInternalFormat::Rgb16F, windowSize);
	m_DepthGbufferAtttachment = new Texture(TextureInternalFormat::DepthComponent16, windowSize);
	
	const std::vector<RenderTargetInfo> attachementsType =
	{
		{
			.attachment = Attachment::Color01,
			.draw = true,
			.isClearing = true
		},
		{
			.attachment = Attachment::Color02,
			.draw = true,
			.isClearing = true
		},
		{
			.attachment = Attachment::Color03,
			.draw = true,
			.isClearing = true
		},
		{
			.attachment = Attachment::Depth,
			.draw = false,
			.isClearing = true
		},
	};
	
	// Set Up renderPass
	const RenderPass renderPass(attachementsType);

	const std::vector<const Texture*> targets = { m_PositionAtttachment, m_NormalAttachement, m_AlbedoAttachment, m_DepthGbufferAtttachment};
	m_GframeBuffer->Create(renderPass, targets);
	
	// Init gbuffer Texture
	m_GBufferShaderLit->Use();

	m_PositionAtttachment->BindTexture(GbufferPosition);
	
	m_NormalAttachement->BindTexture(GbufferNormal);
	
	m_AlbedoAttachment->BindTexture(GbufferAlbedo);
	
	m_GBufferShaderLit->Unuse();
}

void Renderer::InitForwardRenderingAttachment(const Vector2i windowSize)
{
	m_RenderBuffer = new FrameBuffer(windowSize);
	m_ColorAttachment = new Texture(TextureInternalFormat::Rgb16F, m_RenderBuffer->GetSize());
	m_DepthAttachment = new Texture(TextureInternalFormat::DepthComponent16, m_RenderBuffer->GetSize());

	const std::vector<RenderTargetInfo> attachementsType =
	{
		{
			.attachment = Attachment::Color01,
			.draw = true,
			.isClearing = true
		},	
		{
			.attachment = Attachment::Depth,
			.draw = false,
			.isClearing = true
		}
	};
	
	// Set Up renderPass
	const RenderPass renderPass(attachementsType);
	const std::vector<const Texture*> targets = { m_ColorAttachment, m_DepthAttachment };
	m_RenderBuffer->Create(renderPass,targets);
}

void Renderer::DestroyAttachment() const
{
	delete m_GframeBuffer;
	delete m_PositionAtttachment;
	delete m_AlbedoAttachment;
	delete m_NormalAttachement;
	delete m_DepthGbufferAtttachment; 
	
	delete m_RenderBuffer;
	delete m_DepthAttachment;
	delete m_ColorAttachment;
}

void Renderer::DrawLightGizmo(
	const std::vector<const PointLight*>&,
	const std::vector<const SpotLight*>&,
	const std::vector<const DirectionalLight*>&,
	const Camera&
) const
{
}

void Renderer::DefferedRendering(const std::vector<const MeshRenderer*>& meshrenderers, const RendererContext*) const
{
	// Bind for gbuffer pass // 
	m_GframeBuffer->BindFrameBuffer();
	Rhi::ClearColorAndDepth();
	m_GBufferShader->Use();
	DrawMeshRendersByType(meshrenderers, MaterialType::Opaque);
	m_GBufferShader->Unuse();

	// Shading Gbuffer Value //
	m_RenderBuffer->BindFrameBuffer();
	
	// Clear color only
	Rhi::ClearColorAndDepth();
	m_GBufferShaderLit->Use();
	Rhi::DrawQuad(m_Quad->GetId());
	m_GBufferShaderLit->Unuse();
	// END DEFERRED RENDERING
}

void Renderer::ForwardRendering(const std::vector<const MeshRenderer*> meshrenderers,const RendererContext* rendererContext) const
{
	if (rendererContext->isEditor)
	{
		DrawAabb(meshrenderers);
	}
	// TODO draw translucentObject
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
	// EndDeferred

	// Forward
	m_Forward = ResourceManager::Get<Shader>("basic_shader");
	m_Forward->CreateInRhi();

	m_DrawTextureToScreenShader = ResourceManager::Get<Shader>("draw_texture_to_screen");
	m_DrawTextureToScreenShader->CreateInRhi();
	
	m_GizmoShader = ResourceManager::Get<Shader>("gizmo_shader");
	m_GizmoShader->CreateInRhi();

	m_DrawTextureToScreenShader->Use();
	m_DrawTextureToScreenShader->SetInt("BufferTextureId", 0);
	m_DrawTextureToScreenShader->Unuse();
	// EndForward

	// Primitive
	m_Cube = ResourceManager::Get<Model>("assets/models/cube.obj");
	m_Quad = ResourceManager::Get<Model>("assets/models/quad.obj");
	
}

void Renderer::DrawAabb(const std::vector<const MeshRenderer*>& meshRenderers) const
{
	m_GizmoShader->Use();
	Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Line);
	ModelUniformData modelData;

	for (const MeshRenderer* meshRenderer : meshRenderers)
	{
		if (!meshRenderer->model.IsValid())
			continue;

		if (!meshRenderer->drawModelAabb)
			continue;
		
		const Transform& transform =  meshRenderer->entity->transform;
		const ModelAabb&& modelAabb = meshRenderer->model->GetAabb();
		
		const Vector3&& aabbSize = (modelAabb.max - modelAabb.min) * 0.5f;
		const Vector3&& center  = (modelAabb.max + modelAabb.min) * 0.5f;
		
		const Matrix&& trsAabb = Matrix::Trs(center,Quaternion::Identity(),aabbSize);
		modelData.model =  transform.worldMatrix * trsAabb;
		Rhi::UpdateModelUniform(modelData);
		
		Rhi::DrawModel(m_Cube->GetId());
	}
	
	m_GizmoShader->Unuse();
	Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);
}

void Renderer::RenderAllMeshes(const std::vector<const MeshRenderer*>& meshRenderers)
{
	ModelUniformData data;
	
	for (const MeshRenderer* mesh : meshRenderers)
	{
		const Transform& transform = mesh->entity->transform;

		Matrix&& trs = Matrix::Trs(transform.position, transform.rotation, transform.scale);
		data.model = trs;
		data.normalInvertMatrix = trs.Inverted().Transposed();
		
		Rhi::UpdateModelUniform(data);
		Rhi::DrawModel(mesh->model->GetId());
	}
}
