#include "rendering/renderer.hpp"

#include "GLFW/glfw3.h"
#include "rendering/light/directiona_light.hpp"
#include "rendering/light/point_light.hpp"
#include "..\..\include\rendering\light\spot_light.hpp"
#include "resource/resource_manager.hpp"
#include "scene/component/mesh_renderer.hpp"

using namespace XnorCore;

Renderer::Renderer()
	: clearColor(0.5f)
{
	m_Rhi.SetClearColor(clearColor);

	m_VertexPath = FileManager::Get("assets/shaders/vertex.vert");
	m_FragmentPath = FileManager::Get("assets/shaders/fragment.frag");

	m_BasicShader = ResourceManager::Add<Shader>("assets/shaders/shader");
	m_BasicShader->Load(*m_VertexPath, *m_FragmentPath);
	CompileShader();

	m_Rhi.PrepareUniform();
	
}

void Renderer::RenderScene(const Scene& scene, [[maybe_unused]] const RendererContext& rendererContext) const
{
	m_Rhi.SetClearColor(clearColor);
	
	m_Rhi.ClearColorAndDepth();

	if (rendererContext.framebuffer != nullptr)
	{
		rendererContext.framebuffer->BindFrameBuffer();
		m_Rhi.ClearColorAndDepth();
	}
	// SetViewPort
	RHI::SetViewPort(*rendererContext.camera);
	
	m_BasicShader->Use();
	
	CameraUniformData cam;
	cam.cameraPos = rendererContext.camera->pos;
	rendererContext.camera->GetView(&cam.view);
	rendererContext.camera->GetProjection(&cam.projection);
	m_Rhi.UpdateCameraUniform(cam);

	UpdateLight(scene,rendererContext);
	DrawMeshRenders(scene,rendererContext);


	m_BasicShader->UnUse();
	
	if (rendererContext.framebuffer != nullptr)
		rendererContext.framebuffer->UnBindFrameBuffer();
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

void Renderer::UpdateLight(const Scene& scene, const RendererContext&) const
{
	std::vector<const PointLight*> pointLightComponents;
	scene.GetAllComponentOfType<PointLight>(&pointLightComponents);

	std::vector<const SpotLight*> spothLightsComponents;
	scene.GetAllComponentOfType<SpotLight>(&spothLightsComponents);

	std::vector<const Directionalight*> directionalComponent;
	scene.GetAllComponentOfType<Directionalight>(&directionalComponent);

	if (directionalComponent.size() > MaxDirectionalLight)
	{
		Logger::LogWarning("You cannot have more than 1 directional light in the scene");
	}
	
	GpuLightData gpuLightData
	{
		.nbrOfPointLight = static_cast<uint32_t>(pointLightComponents.size()),
		.nbrOfSpotLight = static_cast<uint32_t>(spothLightsComponents.size())
	};

	size_t nbrOfpointLight = pointLightComponents.size();
	nbrOfpointLight = std::clamp(nbrOfpointLight, static_cast<size_t>(0), static_cast<size_t>(MaxPointLight));

	size_t nbrOfspothLight = spothLightsComponents.size();
	nbrOfspothLight = std::clamp(nbrOfspothLight, static_cast<size_t>(0), static_cast<size_t>(MaxSpotLight));

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
		const SpotLight* spotLight = spothLightsComponents[i];
		
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
	RHI::SetPolyGoneMode(PolygonFace::FRONT_AND_BACK, PolygonMode::FILL);

	for (const MeshRenderer* meshRenderer : meshrenderers)
	{
		const Transform& transform =  meshRenderer->entity->transform;
		
		ModelUniformData modelData;
		modelData.model = Matrix::Trs(transform.position, transform.rotation, transform.scale);
		m_Rhi.UpdateModelUniform(modelData);

		if (meshRenderer->texture.IsValid())
			meshRenderer->texture->BindTexture(0);

		if (meshRenderer->model.IsValid())
			RHI::DrawModel(meshRenderer->model->GetId());
	}
}

void Renderer::SetViewPort([[maybe_unused]] const Camera& camera)
{
	RHI::SetViewPort(camera);
}
