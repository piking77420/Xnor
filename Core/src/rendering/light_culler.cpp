#include "rendering/light_culler.hpp"

#include "rendering/rhi.hpp"
#include "rendering/rhi_typedef.hpp"
#include "resource/resource_manager.hpp"
#include "scene/entity.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void LightCuller::InitResources()
{
	m_DirLightTexture = ResourceManager::Get<Texture>("assets_internal/editor/dirlight_icon.png");
	m_PointLightTexture = ResourceManager::Get<Texture>("assets_internal/editor/point_light.png");
	m_SpotLightTexture = ResourceManager::Get<Texture>("assets_internal/editor/spot_light.png");
	m_editorUI = ResourceManager::Get<Shader>("editorui_shader");
	m_editorUI->SetBlendFunction({true,BlendValue::SRC_ALPHA,BlendValue::ONE_MINUS_SRC_ALPHA});
	m_editorUI->CreateInRhi();
	m_editorUI->SetInt("uiTexture",0);
	m_Quad = ResourceManager::Get<Model>("assets/models/quad.obj");
	
}

void LightCuller::UpdateLight(const std::vector<const PointLight*>& pointLightComponents,
                              const std::vector<const SpotLight*>& spotLightsComponents,
                              const std::vector<const DirectionalLight*>& directionalComponent) const
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
	nbrOfpointLight = std::clamp<size_t>(nbrOfpointLight, 0, MaxPointLights);

	size_t nbrOfspothLight = spotLightsComponents.size();
	nbrOfspothLight = std::clamp<size_t>(nbrOfspothLight, 0, MaxSpotLights);

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
		const Matrix matrix = Matrix::Trs(Vector3(0.f), spotLight->entity->transform.rotation.Normalized(), Vector3(1.f));
		const Vector4 direction = matrix * (-Vector4::UnitY());
		
		gpuLightData.spotLightData[i] =
		{
			.color = spotLight->color,
			.intensity = spotLight->intensity,
			.position = spotLight->entity->transform.GetWorldPos(),
			.cutOff = std::cos(spotLight->cutOff),
			.direction = { direction.x, direction.y, direction.z },
			.outerCutOff = std::cos(spotLight->outerCutOff),
		};
	}
	
	gpuLightData.nbrOfSpotLight = static_cast<uint32_t>(nbrOfspothLight);

	if (!directionalComponent.empty())
	{
		const Matrix matrix = Matrix::Trs(Vector3(0.f), directionalComponent[0]->entity->transform.rotation, Vector3(1.f));
		const Vector4 direction = matrix * (-Vector4::UnitY()); 
		
		gpuLightData.directionalData =
		{
			.color = directionalComponent[0]->color,
			.intensity = directionalComponent[0]->intensity,
			.direction = { direction.x, direction.y, direction.z },
		};
	}


	Rhi::UpdateLight(gpuLightData);

}

void LightCuller::DrawLightGizmo(const std::vector<const PointLight*>& pointLightComponents,
	const std::vector<const SpotLight*>& spotLightsComponents,
	const std::vector<const DirectionalLight*>& directionalComponent, const Camera& camera) const
{
	std::map<float_t, GizmoLight> sortedLight;
	
	for (const PointLight* const pointLight : pointLightComponents)
	{
		GizmoLight gizmoLight = {
			.pos = pointLight->entity->transform.position,
			.type = RenderingLight::PointLight,
		};
		
		const float_t distance = (camera.pos - pointLight->entity->transform.position).SquaredLength();
		sortedLight.emplace(distance, gizmoLight);
	}
	
	for (const SpotLight* const spotLight : spotLightsComponents)
	{
		GizmoLight gizmoLight = {
			.pos = spotLight->entity->transform.position,
			.type = RenderingLight::SpothLight
		};
		
		const float_t distance = (camera.pos - spotLight->entity->transform.position).SquaredLength();
		sortedLight.emplace(distance, gizmoLight);
	}
	
	for (const DirectionalLight* const dirLight : directionalComponent)
	{
		GizmoLight gizmoLight = {
			.pos = dirLight->entity->transform.position,
			.type = RenderingLight::DirLight
		};
		
		const float_t distance = (camera.pos - dirLight->entity->transform.position).SquaredLength();
		sortedLight.emplace(distance, gizmoLight);
	}
	
	m_editorUI->Use();
	// ReSharper disable once CppDiscardedPostfixOperatorResult
	for ([[maybe_unused]] std::map<float_t,GizmoLight>::reverse_iterator it = sortedLight.rbegin(); it != sortedLight.rend(); it++)
	{
		ModelUniformData modelData;
		modelData.model = mat4::Trs(it->second.pos,Quaternion::Identity(),vec3(1.f));
		modelData.normalInvertMatrix = mat4::Identity();

		m_PointLightTexture->BindTexture(0);
		
		Rhi::UpdateModelUniform(modelData);
		Rhi::DrawModel(m_Quad->GetId());

	}
	m_editorUI->Unuse();
}
