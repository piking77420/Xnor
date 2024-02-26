#include "rendering/renderer.hpp"

#include "rendering/light/point_light.hpp"
#include "rendering\light\directional_light.hpp"
#include "rendering\light\spot_light.hpp"
#include "resource/resource_manager.hpp"
#include "scene/component/mesh_renderer.hpp"

using namespace XnorCore;

Renderer::Renderer()
	: clearColor(0.5f)
{
}

Renderer::~Renderer()
{
	delete m_RenderBuffer;
	delete m_ColorAttachment;
	delete m_DepthAttachment;
	
	RHI::Shutdown();
}

void Renderer::Initialize()
{
	RHI::SetClearColor(clearColor);

	m_BasicShader = ResourceManager::Get<Shader>("basic_shader");
	m_BasicShader->CreateInRhi();

	m_DrawTextureToScreenShader = ResourceManager::Get<Shader>("draw_texture_to_screen");
	m_DrawTextureToScreenShader->CreateInRhi();
	
	m_GizmoShader = ResourceManager::Get<Shader>("gizmo_shader");
	m_GizmoShader->CreateInRhi();

	m_DrawTextureToScreenShader->Use();
	m_DrawTextureToScreenShader->SetInt("BufferTextureId", 0);
	m_DrawTextureToScreenShader->Unuse();
	
	RHI::PrepareUniform();

	m_Cube = ResourceManager::Get<Model>("assets/models/cube.obj");
	m_Quad = ResourceManager::Get<Model>("assets/models/quad.obj");
}

void Renderer::RenderScene(const Scene& scene, const RendererContext& rendererContext) const
{
	// Clear MainWindow // 
	RHI::SetClearColor(clearColor);
	RHI::ClearColorAndDepth();

	rendererContext.framebuffer->BindFrameBuffer();
	RHI::SetClearColor(clearColor);
	RHI::ClearColorAndDepth();

	m_RenderBuffer->BindFrameBuffer();
	RHI::SetClearColor(clearColor);
	RHI::ClearColorAndDepth();
	
	RHI::SetViewport(m_RenderBuffer->GetSize());
	
	m_BasicShader->Use();
	CameraUniformData cam;
	cam.cameraPos = rendererContext.camera->pos;
	rendererContext.camera->GetView(&cam.view);
	rendererContext.camera->GetProjection(rendererContext.framebuffer->GetSize(), &cam.projection);
	
	RHI::UpdateCameraUniform(cam);

	UpdateLight(scene,rendererContext);
	DrawMeshRenders(scene,rendererContext);
	m_BasicShader->Unuse();
	m_RenderBuffer->UnBindFrameBuffer();
	
	if (rendererContext.framebuffer != nullptr)
	{
		rendererContext.framebuffer->BindFrameBuffer();
		RHI::SetViewport(rendererContext.framebuffer->GetSize());

		// Render To Imgui frame buffer
		m_DrawTextureToScreenShader->Use();
		m_ColorAttachment->BindTexture(0);
		
		RHI::DrawQuad(m_Quad->GetId());
		m_DrawTextureToScreenShader->Unuse();
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

void Renderer::PrepareRendering(vec2i windowSize)
{
	
	m_RenderBuffer = new FrameBuffer(windowSize);
	
	
	m_ColorAttachment = new Texture(TextureInternalFormat::Rgba16F, m_RenderBuffer->GetSize());
	m_DepthAttachment = new Texture(TextureInternalFormat::DepthStencil,m_RenderBuffer->GetSize());

	const std::vector<RenderTargetInfo> attachementsType =
	{
		{Attachment::Color_Attachment01,true},
		{Attachment::DepthAndStencil,true},
	};
	
	
	// Set Up renderPass
	const RenderPass renderPass(attachementsType);
	const std::vector<const Texture*> targets = { m_ColorAttachment, m_DepthAttachment };
	m_RenderBuffer->Create(renderPass,targets);
	
}

void Renderer::UpdateLight(const Scene& scene, const RendererContext&) const
{
	std::vector<const PointLight*> pointLightComponents;
	scene.GetAllComponentOfType<PointLight>(&pointLightComponents);

	std::vector<const SpotLight*> spotLightsComponents;
	scene.GetAllComponentOfType<SpotLight>(&spotLightsComponents);

	std::vector<const DirectionalLight*> directionalComponent;
	scene.GetAllComponentOfType<DirectionalLight>(&directionalComponent);

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
			.position = pointLight->entity->transform.position,
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
			.position = spotLight->entity->transform.position,
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

void Renderer::DrawMeshRenders(const Scene& scene, const RendererContext&) const 
{
	std::vector<const MeshRenderer*> meshrenderers;
	scene.GetAllComponentOfType<MeshRenderer>(&meshrenderers);
	RHI::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);

	for (const MeshRenderer* meshRenderer : meshrenderers)
	{
		Transform& transform = meshRenderer->entity->transform;
		ModelUniformData modelData;
		
		modelData.model = Matrix::Trs(transform.position, transform.quaternion, transform.scale);
		RHI::UpdateModelUniform(modelData);

		if (meshRenderer->texture.IsValid())
			meshRenderer->texture->BindTexture(0);

		if (meshRenderer->model.IsValid())
			RHI::DrawModel(meshRenderer->model->GetId());
		
	}
	
	m_GizmoShader->Use();
	RHI::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Line);
	// Draw AABB 
	for (const MeshRenderer* meshRenderer : meshrenderers)
	{
		if (!meshRenderer->model.IsValid())
			continue;

		if (!meshRenderer->drawModelAabb)
			continue;
		
		const Transform& transform =  meshRenderer->entity->transform;
		const ModelAABB&& modelAabb = meshRenderer->model->GetAABB();
		
		Vector3 aabbMinMax = (modelAabb.max - modelAabb.min) * 0.5f;
		Vector3 aabbSize = {aabbMinMax.x * transform.scale.x , aabbMinMax.y * transform.scale.y, aabbMinMax.z * transform.scale.z};
		
		ModelUniformData modelData;
		modelData.model = Matrix::Trs(transform.position, transform.quaternion.Normalized(), aabbSize);
		RHI::UpdateModelUniform(modelData);
		
		RHI::DrawModel(m_Cube->GetId());
	}
	
	m_GizmoShader->Unuse();
	RHI::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);

}

