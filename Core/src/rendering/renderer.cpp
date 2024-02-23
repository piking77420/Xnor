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
	m_Rhi.SetClearColor(clearColor);

	m_VertexPath = FileManager::Get("assets/shaders/vertex.vert");
	m_FragmentPath = FileManager::Get("assets/shaders/fragment.frag");
	m_BasicShader = ResourceManager::Add<Shader>("mainShader");
	m_BasicShader->Load(*m_VertexPath, *m_FragmentPath);

	m_VertexDrawTextureToScreenPath = FileManager::Get("assets/shaders/DrawTextureToScreen/DrawTextureToScreen.vert");
	DrawTextureToScreen = FileManager::Get("assets/shaders/DrawTextureToScreen/DrawTextureToScreen.frag");
	m_DrawTextureToScreenShader = ResourceManager::Add<Shader>("DrawTextureToScreenShader");
	m_DrawTextureToScreenShader->Load(*m_VertexDrawTextureToScreenPath, *DrawTextureToScreen);
	
	m_GizmoShader = ResourceManager::Add<Shader>("GizmoShader");
	m_GizmoShader->Load(*FileManager::Get("assets/shaders/GizmoShader/GizmoShader.vert"),*FileManager::Get("assets/shaders/GizmoShader/GizmoShader.frag"));

	m_DrawTextureToScreenShader->Use();
	m_DrawTextureToScreenShader->SetInt("BufferTextureId",0);
	m_DrawTextureToScreenShader->UnUse();
	
	m_Rhi.PrepareUniform();

	m_Cube = ResourceManager::Load<Model>(FileManager::Get("assets/models/cube.obj"));
	m_Quad = ResourceManager::Load<Model>(FileManager::Get("assets/models/quad.obj"));
}

Renderer::~Renderer()
{
	delete m_RenderBuffer;
	delete m_ColorAttachment;
	delete m_DepthAttachment;
}

void Renderer::RenderScene(const Scene& scene, [[maybe_unused]] const RendererContext& rendererContext) const
{
	// Clear MainWindow // 
	Vector2i screenSize = Window::GetSize();
	m_Rhi.SetClearColor(clearColor);
	m_Rhi.ClearColorAndDepth();

	rendererContext.framebuffer->BindFrameBuffer();
	m_Rhi.SetClearColor(clearColor);
	m_Rhi.ClearColorAndDepth();

	m_RenderBuffer->BindFrameBuffer();
	m_Rhi.SetClearColor(clearColor);
	m_Rhi.ClearColorAndDepth();
	
	RHI::SetViewport(m_RenderBuffer->GetSize());
	
	m_BasicShader->Use();
	CameraUniformData cam;
	cam.cameraPos = rendererContext.camera->pos;
	rendererContext.camera->GetView(&cam.view);
	rendererContext.camera->GetProjection(rendererContext.framebuffer->GetSize(),&cam.projection);
	m_Rhi.UpdateCameraUniform(cam);

	UpdateLight(scene,rendererContext);
	DrawMeshRenders(scene,rendererContext);
	m_BasicShader->UnUse();
	m_RenderBuffer->UnBindFrameBuffer();
	
	if (rendererContext.framebuffer != nullptr)
	{
		rendererContext.framebuffer->BindFrameBuffer();
		RHI::SetViewport(rendererContext.framebuffer->GetSize());

		// Render To Imgui frame buffer
		m_DrawTextureToScreenShader->Use();
		m_ColorAttachment->BindTexture(0);
		
		RHI::DrawQuad(m_Quad->GetId());
		m_DrawTextureToScreenShader->UnUse();
		rendererContext.framebuffer->UnBindFrameBuffer();
	}

}

void Renderer::CompileShader()
{
	if (m_BasicShader->IsLoaded())
	{
		m_BasicShader->Recompile(*m_VertexPath, *m_FragmentPath);
	}
	else
	{
		m_BasicShader->Load(*m_VertexPath, *m_FragmentPath);
	}
}

void Renderer::OnResizeWindow()
{
	
}

void Renderer::SwapBuffers()
{
	RHI::SwapBuffers();
}

void Renderer::PrepareRendering(vec2i windowSize)
{
	
	m_RenderBuffer = new FrameBuffer(windowSize);
	
	const std::vector attachementsType =
	{
		AttachementsType::Color,
		AttachementsType::DepthAndStencil
	};
	
	m_ColorAttachment = new Texture(AttachementsType::Color, m_RenderBuffer->GetSize());
	m_DepthAttachment = new Texture(AttachementsType::DepthAndStencil,m_RenderBuffer->GetSize());
    
	// Set Up renderPass
	const RenderPass renderPass(attachementsType);
	const std::vector targets = { m_ColorAttachment, m_DepthAttachment };
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
		
		gpuLightData.spotLightData[i] =
		{
			.color = spotLight->color,
			.intensity = spotLight->intensity,
			.position = spotLight->entity->transform.position,
			.cutOff = spotLight->cutOff,
			.direction = Vector3::Zero(),
			.outerCutOff = spotLight->outerCutOff,
		};
	}
	gpuLightData.nbrOfSpotLight = static_cast<uint32_t>(nbrOfspothLight);

	if (!directionalComponent.empty())
	{
		gpuLightData.directionalData =
		{
			.color = directionalComponent[0]->color,
			.intensity = directionalComponent[0]->intensity,
			.direction = directionalComponent[0]->direction,
		};
	}

	m_Rhi.UpdateLight(gpuLightData);
}

void Renderer::DrawMeshRenders(const Scene& scene, [[maybe_unused]] const RendererContext& rendererContext) const 
{
	std::vector<const MeshRenderer*> meshrenderers;
	scene.GetAllComponentOfType<MeshRenderer>(&meshrenderers);
	RHI::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);

	for (const MeshRenderer* meshRenderer : meshrenderers)
	{
		Transform& transform = meshRenderer->entity->transform;
		
		ModelUniformData modelData;
		
		transform.quaternion = Quaternion::FromEuler(transform.rotation);
		modelData.model = Matrix::Trs(transform.position, transform.quaternion, transform.scale);
		m_Rhi.UpdateModelUniform(modelData);

		if (meshRenderer->texture.IsValid())
			meshRenderer->texture->BindTexture(0);

		if (meshRenderer->model.IsValid())
			RHI::DrawModel(meshRenderer->model->GetId());
		
	}
	/*
	m_GizmoShader->Use();
	RHI::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Line);
	// Draw AABB 
	for (const MeshRenderer* meshRenderer : meshrenderers)
	{
		if (!meshRenderer->model.IsValid())
			continue;

		if(!meshRenderer->m_DrawModelAABB)
			continue;
		const Transform& transform =  meshRenderer->entity->transform;

		ModelUniformData modelData;
		modelData.model = Matrix::Trs(transform.position, transform.rotation, transform.scale);
		m_Rhi.UpdateModelUniform(modelData);
		
		RHI::DrawModel(m_Cube->GetId());
	}
	m_GizmoShader->UnUse();*/
}

