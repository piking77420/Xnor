#include "rendering/render_systems/light_manager.hpp"

#include "rendering/rhi.hpp"
#include "rendering/rhi_typedef.hpp"
#include "resource/resource_manager.hpp"
#include "scene/entity.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void LightManager::InitResources()
{
	m_DirLightTexture = ResourceManager::Get<Texture>("assets_internal/editor/dirlight_icon.png");
	m_PointLightTexture = ResourceManager::Get<Texture>("assets_internal/editor/point_light.png");
	m_SpotLightTexture = ResourceManager::Get<Texture>("assets_internal/editor/spot_light.png");
	m_EditorUi = ResourceManager::Get<Shader>("editor_ui_shader");

	constexpr BlendFunction blendFunction = {
		.isBlending = true,
		.sValue = BlendValue::SrcAlpha,
		.dValue = BlendValue::OneMinusSrcAlpha
	};
	
	m_EditorUi->SetBlendFunction(blendFunction);
	m_EditorUi->CreateInRhi();
	m_EditorUi->SetInt("uiTexture",0);
	m_Quad = ResourceManager::Get<Model>("assets/models/quad.obj");
}

void LightManager::BeginFrame(const Scene& scene)
{
	scene.GetAllComponentOfType<PointLight>(&pointLights);
	scene.GetAllComponentOfType<SpotLight>(&spotLights);
	scene.GetAllComponentOfType<DirectionalLight>(&directionalLights);
	
    if (directionalLights.size() > MaxDirectionalLights)
		Logger::LogWarning("You cannot have more than 1 directional light in the scene");

	GpuLightData gpuLightData
	{
		.nbrOfPointLight = static_cast<uint32_t>(pointLights.size()),
		.nbrOfSpotLight = static_cast<uint32_t>(spotLights.size())
	};

	const size_t nbrOfpointLight = std::clamp<size_t>(pointLights.size(), 0, MaxPointLights);
	const size_t nbrOfSpotLight = std::clamp<size_t>(spotLights.size(), 0, MaxSpotLights);
	const size_t nbrOfDirectionalLight = std::clamp<size_t>(directionalLights.size(), 0, MaxSpotLights);

	for (size_t i = 0; i < nbrOfpointLight; i++)
	{
		const PointLight* pointLight = pointLights[i];
		
		gpuLightData.pointLightData[i] =
		{
			.color = pointLight->color.Rgb(),
			.intensity = pointLight->intensity,
			.position = static_cast<Vector3>(pointLight->entity->transform.worldMatrix[3]),
			.radius = 30.f * sqrt(pointLight->intensity),
		};
	}
	gpuLightData.nbrOfPointLight = static_cast<uint32_t>(nbrOfpointLight);

	for (size_t i = 0 ; i < nbrOfSpotLight ; i++)
	{
		const SpotLight* spotLight = spotLights[i];
		const Matrix matrix = Matrix::Trs(Vector3(0.f), spotLight->entity->transform.GetRotation().Normalized(), Vector3(1.f));
		const Vector4 direction = matrix * -Vector4::UnitY();
		
		gpuLightData.spotLightData[i] =
		{
			.color = spotLight->color.Rgb(),
			.intensity = spotLight->intensity,
			.position = static_cast<Vector3>(spotLight->entity->transform.worldMatrix[3]),
			.cutOff = std::cos(spotLight->cutOff),
			.direction = { direction.x, direction.y, direction.z },
			.outerCutOff = std::cos(spotLight->outerCutOff),
		};
	}
	
	gpuLightData.nbrOfSpotLight = static_cast<uint32_t>(nbrOfSpotLight);

	for (size_t i = 0 ; i < nbrOfDirectionalLight ; i++)
	{
		const Matrix matrix = Matrix::Trs(Vector3(0.f), directionalLights[i]->entity->transform.GetRotation(), Vector3(1.f));
		const Vector4 direction = matrix * Vector4::UnitY(); 
		
		gpuLightData.directionalData[i] =
		{
			.color = directionalLights[i]->color.Rgb(),
			.intensity = directionalLights[i]->intensity,	
			.direction = { direction.x, direction.y, direction.z },
		};
	}

	Rhi::UpdateLight(gpuLightData);
}

void LightManager::EndFrame(const Scene&)
{
	pointLights.clear();
	spotLights.clear();
	directionalLights.clear();
}

void LightManager::DrawLightGizmo(const Camera& camera, const Scene& scene) const
{
	DrawLightGizmoWithShader(camera,scene,m_EditorUi);
}

void LightManager::DrawLightGizmoWithShader(const Camera& camera, const Scene& scene,const Pointer<Shader>& shader) const
{
	shader->Use();
	
	std::map<float_t, GizmoLight> sortedLight;
	
	for (const PointLight* const pointLight : pointLights)
	{
		GizmoLight gizmoLight = {
			.pos = pointLight->entity->transform.GetPosition(),
			.light = pointLight,
			.type = RenderingLight::PointLight,
		};
		
		const float_t distance = (camera.position - pointLight->entity->transform.GetPosition()).SquaredLength();
		sortedLight.emplace(distance, gizmoLight);
	}
	
	for (const SpotLight* const spotLight : spotLights)
	{
		GizmoLight gizmoLight = {
			.pos = spotLight->entity->transform.GetPosition(),
			.light = spotLight,
			.type = RenderingLight::SpothLight
		};
		
		const float_t distance = (camera.position - spotLight->entity->transform.GetPosition()).SquaredLength();
		sortedLight.emplace(distance, gizmoLight);
	}
	
	for (const DirectionalLight* const dirLight : directionalLights)
	{
		GizmoLight gizmoLight = {
			.pos = dirLight->entity->transform.GetPosition(),
			.light = dirLight,
			.type = RenderingLight::DirLight
		};
		
		const float_t distance = (camera.position - dirLight->entity->transform.GetPosition()).SquaredLength();
		sortedLight.emplace(distance, gizmoLight);
	}
	
	
	for (decltype(sortedLight)::reverse_iterator it = sortedLight.rbegin(); it != sortedLight.rend(); it++)
	{
		ModelUniformData modelData;
		float_t scaleScalar = m_ScaleFactor;

		float_t distance = (it->second.pos - camera.position).SquaredLength();
		if (distance < m_MinDistance * m_MinDistance)
			scaleScalar = scaleScalar * (1.f / distance) * (1.f / distance);

		scaleScalar = std::clamp(scaleScalar, m_MaxScalarFactor, m_MinScalarFactor);
		Matrix scale = Matrix::Scaling(Vector3(scaleScalar));
		modelData.model = (scale * Matrix::LookAt(it->second.pos, camera.position, Vector3::UnitY())).Inverted();
		modelData.normalInvertMatrix = Matrix::Identity();
		// +1 to avoid the black color of the attachement be a valid index  
		modelData.meshRenderIndex = scene.GetEntityIndex(it->second.light->entity) + 1;
		
		switch (it->second.type)
		{
			case RenderingLight::PointLight:
				m_PointLightTexture->BindTexture(0);
				break;
			
			case RenderingLight::SpothLight:
				m_SpotLightTexture->BindTexture(0);
				break;
			
			case RenderingLight::DirLight:
				m_DirLightTexture->BindTexture(0);
				break;
		}
		
		Rhi::UpdateModelUniform(modelData);
		Rhi::DrawModel(m_Quad->GetId());
	}

	shader->Unuse();
}
