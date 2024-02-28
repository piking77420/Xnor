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

Renderer::~Renderer()
{
}

void Renderer::Initialize()
{
	RHI::SetClearColor(clearColor);

	InitResources();

	RHI::PrepareUniform();

}

void Renderer::Shutdown()
{
	DestroyAttachment();
}

void Renderer::RenderScene(const World& world, const RendererContext& rendererContext) const
{
	const Scene& scene = world.Scene;
	
	std::vector<const MeshRenderer*> meshrenderers;
	scene.GetAllComponentOfType<MeshRenderer>(&meshrenderers);
	
	std::vector<const PointLight*> pointLights;
	std::vector<const SpotLight*> spotLights;
	std::vector<const DirectionalLight*> directionalLights;
	scene.GetAllComponentOfType<PointLight>(&pointLights);
	scene.GetAllComponentOfType<SpotLight>(&spotLights);
	scene.GetAllComponentOfType<DirectionalLight>(&directionalLights);

	// Update Light
	UpdateLight(pointLights,spotLights,directionalLights);
	
	// Update Camera
	CameraUniformData cam;
	rendererContext.camera->GetView(&cam.view);
	rendererContext.camera->GetProjection(rendererContext.framebuffer->GetSize(), &cam.projection);
	cam.cameraPos = rendererContext.camera->pos;
	RHI::UpdateCameraUniform(cam);

	RHI::SetViewport(m_RenderBuffer->GetSize());
	// Clear MainWindow // 
	RHI::ClearColorAndDepth();

	
	// Bind for gbuffer pass // 
	m_GframeBuffer->BindFrameBuffer();
	RHI::ClearColorAndDepth();
	m_gBufferShader->Use();
	DrawMeshRendersByType(meshrenderers,MaterialType::Opaque);
	m_gBufferShader->Unuse();

	
	// Shading Gbuffer Value //
	m_RenderBuffer->BindFrameBuffer();
	
	// Clear color only
	RHI::ClearColorAndDepth();
	m_gBufferShaderLit->Use();
	RHI::DrawQuad(m_Quad->GetId());
	m_gBufferShaderLit->Unuse();
	// END DEFFERED RENDERING

	// Blit depth of gbuffer to forward Pass
	RHI::BlitFrameBuffer(m_GframeBuffer->GetId(),m_RenderBuffer->GetId(),
		{0,0},m_GframeBuffer->GetSize(),
		{0,0},m_RenderBuffer->GetSize(),Attachment::Depth,TextureFiltering::Nearest);
	
	// ForwardPass //
	DrawAABB(meshrenderers);
	m_RenderBuffer->UnBindFrameBuffer();
	
	// DRAW THE FINAL IMAGE TEXTURE
	if (rendererContext.framebuffer != nullptr)
	{
		rendererContext.framebuffer->BindFrameBuffer();
		
		RHI::ClearColorAndDepth();
		RHI::SetViewport(rendererContext.framebuffer->GetSize());
	}
	m_DrawTextureToScreenShader->Use();
	m_ColorAttachment->BindTexture(0);
	RHI::DrawQuad(m_Quad->GetId());
	m_DrawTextureToScreenShader->Unuse();
	
	if(rendererContext.framebuffer != nullptr)
	{
		rendererContext.framebuffer->UnBindFrameBuffer();
	}
}

void Renderer::CompileShader()
{
	if (m_BasicShader->GetLoaded())
	{
		m_BasicShader->Recompile();
	}
	else
	{
		m_BasicShader->CreateInRhi();
	}
}

void Renderer::SwapBuffers()
{
	RHI::SwapBuffers();
}

void Renderer::OnResize(vec2i windowSize)
{
	DestroyAttachment();
	InitDefferedRenderingAttachment(windowSize);
	InitForwardRenderingAttachment(windowSize);
}

void Renderer::PrepareRendering(vec2i windowSize)
{
	InitDefferedRenderingAttachment(windowSize);
	InitForwardRenderingAttachment(windowSize);
}

void Renderer::UpdateLight(const std::vector<const PointLight*>& pointLightComponents,
	const std::vector<const SpotLight*>& spotLightsComponents,const std::vector<const DirectionalLight*>& directionalComponent) const
{
	
	if (directionalComponent.size() > MaxDirectionalLights)
	{
		Logger::LogWarning("You cannot have more than 1 directional light in the scene");
	}
	
	GpuLightData gpuLightData
	{
		.nbrOfPointLight = static_cast<uint32_t>(pointLightComponents.size()),
		.nbrOfSpotLight = static_cast<uint32_t>(spotLightsComponents.size())
	};

	size_t nbrOfpointLight = pointLightComponents.size();
	nbrOfpointLight = std::clamp(nbrOfpointLight, static_cast<size_t>(0), static_cast<size_t>(MaxPointLights));

	size_t nbrOfspothLight = spotLightsComponents.size();
	nbrOfspothLight = std::clamp(nbrOfspothLight, static_cast<size_t>(0), static_cast<size_t>(MaxSpotLights));

	for (size_t i = 0; i < nbrOfpointLight; i++)
	{
		const PointLight* pointLight = pointLightComponents[i];
		
		gpuLightData.pointLightData[i] =
		{
			.color = pointLight->color,
			.intensity = pointLight->intensity,
			.position = pointLight->entity->transform.GetWorldPos(),
			.radius = 30.f * sqrt(pointLight->intensity),
		};
	}
	gpuLightData.nbrOfPointLight = static_cast<uint32_t>(nbrOfpointLight);

	for (size_t i = 0 ; i < nbrOfspothLight ; i++)
	{
		const SpotLight* spotLight = spotLightsComponents[i];
		const Matrix matrix = Matrix::Trs(Vector3(0.f),spotLight->entity->transform.quaternion.Normalized(),Vector3(1.f));
		Vector4 direction = matrix * (-Vector4::UnitY());
		
		gpuLightData.spotLightData[i] =
		{
			.color = spotLight->color,
			.intensity = spotLight->intensity,
			.position = spotLight->entity->transform.GetWorldPos(),
			.cutOff = std::cos(spotLight->cutOff),
			.direction = {direction.x,direction.y,direction.z},
			.outerCutOff = std::cos(spotLight->outerCutOff),
		};
	}
	gpuLightData.nbrOfSpotLight = static_cast<uint32_t>(nbrOfspothLight);

	if (!directionalComponent.empty())
	{
		const Matrix matrix = Matrix::Trs(Vector3(0.f),directionalComponent[0]->entity->transform.quaternion,Vector3(1.f));
		Vector4 direction = matrix * (-Vector4::UnitY()); 
		
		gpuLightData.directionalData =
		{
			.color = directionalComponent[0]->color,
			.intensity = directionalComponent[0]->intensity,
			.direction = {direction.x,direction.y,direction.z},
		};
	}

	RHI::UpdateLight(gpuLightData);
}

void Renderer::DrawLightGizmo(
	const std::vector<const PointLight*>& pointLightComponents,
	const std::vector<const SpotLight*>& spotLightsComponents,
	const  std::vector<const DirectionalLight*>& directionalComponent,const Camera& camera
) const
{

	std::map<float_t,GizmoLight> sortedLight;
	
	for (uint32_t i = 0; i < pointLightComponents.size(); i++)
	{
		GizmoLight gizmoLight;
		gizmoLight.pos = pointLightComponents[i]->entity->transform.position;
		gizmoLight.type = RenderingLight::PointLight;
		
		float_t distance = (camera.pos - pointLightComponents[i]->entity->transform.position).SquaredLength();
		sortedLight.emplace(distance,gizmoLight);
	}
	for (uint32_t i = 0; i < spotLightsComponents.size(); i++)
	{
		GizmoLight gizmoLight;
		gizmoLight.pos = pointLightComponents[i]->entity->transform.position;
		gizmoLight.type = RenderingLight::SpothLight;
		
		float_t distance = (camera.pos - pointLightComponents[i]->entity->transform.position).SquaredLength();
		sortedLight.emplace(distance,gizmoLight);
	}
	for (uint32_t i = 0; i < directionalComponent.size(); i++)
	{
		GizmoLight gizmoLight;
		gizmoLight.pos = pointLightComponents[i]->entity->transform.position;
		gizmoLight.type = RenderingLight::DirLight;
		
		float_t distance = (camera.pos - pointLightComponents[i]->entity->transform.position).SquaredLength();
		sortedLight.emplace(distance,gizmoLight);
	}

	for(std::map<float_t,GizmoLight>::reverse_iterator it = sortedLight.rbegin(); it != sortedLight.rend(); ++it)
	{
		
	}
	
}

void Renderer::DrawMeshRendersByType(const std::vector<const MeshRenderer*>& meshRenderers, MaterialType materialtype
) const
{
	RHI::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);

	for (const MeshRenderer* meshRenderer : meshRenderers)
	{
		if(meshRenderer->material.materialType != materialtype)
			continue;
		
		Transform& transform = meshRenderer->entity->transform;
		ModelUniformData modelData;

		modelData.model = transform.worldMatrix;
		modelData.normalInvertMatrix = transform.worldMatrix.Inverted().Transposed();
		RHI::UpdateModelUniform(modelData);

		if (meshRenderer->material.textures.IsValid())
			meshRenderer->material.textures->BindTexture(0);

		if (meshRenderer->model.IsValid())
			RHI::DrawModel(meshRenderer->model->GetId());
		
	}
}


void Renderer::InitDefferedRenderingAttachment(vec2i windowSize)
{
	
	m_GframeBuffer = new FrameBuffer(windowSize);

	m_PositionAtttachment = new Texture(TextureInternalFormat::Rgb16F,windowSize);
	m_NormalAttachement = new Texture(TextureInternalFormat::Rgb16F,windowSize);
	m_AlbedoAtttachment = new Texture(TextureInternalFormat::Rgb16F,windowSize);
	m_DepthGbufferAtttachment = new Texture(TextureInternalFormat::DepthComponent16,windowSize);
	
	const std::vector<RenderTargetInfo> attachementsType =
	{
		{Attachment::Color_Attachment01,true,true},
		{Attachment::Color_Attachment02,true,true},
		{Attachment::Color_Attachment03,true,true},
		{Attachment::Depth,false,true},

	};
	// Set Up renderPass
	const RenderPass renderPass(attachementsType);

	const std::vector<const Texture*> targets = { m_PositionAtttachment,m_NormalAttachement,m_AlbedoAtttachment,m_DepthGbufferAtttachment};
	m_GframeBuffer->Create(renderPass,targets);
	
	// Init gbuffer Texture
	m_gBufferShaderLit->Use();

	m_PositionAtttachment->BindTexture(GbufferPosition);
	
	m_NormalAttachement->BindTexture(GbufferNormal);
	
	m_AlbedoAtttachment->BindTexture(GbufferAlbedo);
	
	m_gBufferShaderLit->Unuse();
	
}

void Renderer::InitForwardRenderingAttachment(vec2i windowSize)
{
	m_RenderBuffer = new FrameBuffer(windowSize);
	m_ColorAttachment = new Texture(TextureInternalFormat::Rgb16F, m_RenderBuffer->GetSize());
	m_DepthAttachment = new Texture(TextureInternalFormat::DepthComponent16,m_RenderBuffer->GetSize());

	const std::vector<RenderTargetInfo> attachementsType =
	{
		{Attachment::Color_Attachment01,true,true},
		{Attachment::Depth,false,true},
	};
	// Set Up renderPass
	const RenderPass renderPass(attachementsType);
	const std::vector<const Texture*> targets = { m_ColorAttachment,m_DepthAttachment };
	m_RenderBuffer->Create(renderPass,targets);
}

void Renderer::DestroyAttachment()
{
	delete m_GframeBuffer;
	delete m_PositionAtttachment;
	delete m_AlbedoAtttachment;
	delete m_NormalAttachement;
	delete m_DepthGbufferAtttachment; 
	
	delete m_RenderBuffer;
	delete m_DepthAttachment;
	delete m_ColorAttachment;
}

void Renderer::InitResources()
{
	// Deffered 
	m_gBufferShader = ResourceManager::Get<Shader>("gbuffer");
	m_gBufferShaderLit = ResourceManager::Get<Shader>("deffered_opaque");
	m_gBufferShader->CreateInRhi();
	m_gBufferShaderLit->CreateInRhi();
	m_gBufferShaderLit->Use();
	m_gBufferShaderLit->SetInt("gPosition",GbufferPosition);
	m_gBufferShaderLit->SetInt("gNormal",GbufferNormal);
	m_gBufferShaderLit->SetInt("gAlbedoSpec",GbufferAlbedo);
	m_gBufferShaderLit->Unuse();
	// Init diffuse Texture for gbuffer
	m_gBufferShader->Use();
	m_gBufferShader->SetInt("textureDiffuse",0);
	m_gBufferShader->Unuse();
	// EndDefferd

	// Forward
	m_BasicShader = ResourceManager::Get<Shader>("basic_shader");
	m_BasicShader->CreateInRhi();

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

void Renderer::DrawAABB(const std::vector<const MeshRenderer*>& meshRenderers) const
{
	m_GizmoShader->Use();
	RHI::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Line);
	ModelUniformData modelData;

	for (const MeshRenderer* meshRenderer : meshRenderers)
	{
		if (!meshRenderer->model.IsValid())
			continue;

		if (!meshRenderer->drawModelAabb)
			continue;
		
		const Transform& transform =  meshRenderer->entity->transform;
		const ModelAABB&& modelAabb = meshRenderer->model->GetAABB();
		
		const Vector3 aabbMinMax = (modelAabb.max - modelAabb.min) * 0.5f;
		const Vector3 aabbSize = {aabbMinMax.x * transform.scale.x , aabbMinMax.y * transform.scale.y, aabbMinMax.z * transform.scale.z};
		const Matrix&& scaleMatrix = Matrix::Scaling(aabbSize);
		
		modelData.model = scaleMatrix * transform.worldMatrix;
		RHI::UpdateModelUniform(modelData);
		
		RHI::DrawModel(m_Cube->GetId());
	}
	
	m_GizmoShader->Unuse();
	RHI::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);

}

void Renderer::RenderAllMeshes(const std::vector<const MeshRenderer*>& meshRenderers)
{
	ModelUniformData data;
	
	for (const MeshRenderer* mesh : meshRenderers)
	{
		const Transform& transform = mesh->entity->transform;

		Matrix&& trs = Matrix::Trs(transform.position, transform.quaternion, transform.scale);
		data.model = trs;
		data.normalInvertMatrix = trs.Inverted().Transposed();
		
		RHI::UpdateModelUniform(data);
		RHI::DrawModel(mesh->model->GetId());
	}
	
}

void Renderer::ShadowPathSpotLight([[maybe_unused]]const std::vector<const SpotLight*>& spotLights)
{

	
	
}

