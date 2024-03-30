#include "rendering/render_systems/light_manager.hpp"

#include "rendering/rhi.hpp"
#include "rendering/rhi_typedef.hpp"
#include "resource/resource_manager.hpp"
#include "scene/entity.hpp"
#include "utils/logger.hpp"
#include "rendering/renderer.hpp"

using namespace XnorCore;

LightManager::~LightManager()
{
	for (const DirectionalShadowMap directionalShadowMap : directionalShadowMaps)
	{
		delete directionalShadowMap.depthTexture;
	}
	delete m_GpuLightData;
	delete m_ShadowMapTextureArray;
}

void LightManager::InitResources()
{
	m_GpuLightData = new GpuLightData();
	
	m_DirLightTexture = ResourceManager::Get<Texture>("assets_internal/editor/gizmos/dirlight_icon.png");
	m_PointLightTexture = ResourceManager::Get<Texture>("assets_internal/editor/gizmos/point_light.png");
	m_SpotLightTexture = ResourceManager::Get<Texture>("assets_internal/editor/gizmos/spot_light.png");
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
	
	InitShadow();
}

void LightManager::BeginFrame(const Scene& scene, const Renderer& renderer)
{
	scene.GetAllComponentOfType<PointLight>(&pointLights);
	scene.GetAllComponentOfType<SpotLight>(&spotLights);
	scene.GetAllComponentOfType<DirectionalLight>(&directionalLights);
	FecthLightInfo();
	ComputeShadow(scene, renderer);
	Rhi::UpdateLight(*m_GpuLightData);

}

void LightManager::EndFrame(const Scene&)
{
}

void LightManager::DrawLightGizmo(const Camera& camera, const Scene& scene)
{
	scene.GetAllComponentOfType<PointLight>(&pointLights);
	scene.GetAllComponentOfType<SpotLight>(&spotLights);
	scene.GetAllComponentOfType<DirectionalLight>(&directionalLights);
	DrawLightGizmoWithShader(camera, scene, m_EditorUi);
}

void LightManager::DrawLightGizmoWithShader(const Camera& camera, const Scene& scene,const Pointer<Shader>& shader) const
{
	shader->Use();
	
	std::map<float_t, GizmoLight> sortedLight;
	
	for (const PointLight* const pointLight : pointLights)
	{
		if (pointLight == nullptr)
			continue;
		
		GizmoLight gizmoLight = {
			.pos = pointLight->GetEntity()->transform.GetPosition(),
			.light = pointLight,
			.type = RenderingLight::PointLight,
		};
		
		const float_t distance = (camera.position - pointLight->GetEntity()->transform.GetPosition()).SquaredLength();
		sortedLight.emplace(distance, gizmoLight);
	}
	
	for (const SpotLight* const spotLight : spotLights)
	{
		if (spotLight == nullptr)
			continue;
		
		GizmoLight gizmoLight = {
			.pos = spotLight->GetEntity()->transform.GetPosition(),
			.light = spotLight,
			.type = RenderingLight::SpothLight
		};
		
		const float_t distance = (camera.position - spotLight->GetEntity()->transform.GetPosition()).SquaredLength();
		sortedLight.emplace(distance, gizmoLight);
	}
	
	for (const DirectionalLight* const dirLight : directionalLights)
	{
		if (dirLight == nullptr)
			continue;
		
		GizmoLight gizmoLight = {
			.pos = dirLight->GetEntity()->transform.GetPosition(),
			.light = dirLight,
			.type = RenderingLight::DirLight
		};
		
		const float_t distance = (camera.position - dirLight->GetEntity()->transform.GetPosition()).SquaredLength();
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
		modelData.meshRenderIndex = scene.GetEntityIndex(it->second.light->GetEntity()) + 1;
		
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

void LightManager::BindShadowMap() const
{
	for (uint32_t i = 0; i < directionalLights.size(); i++)
	{
		if (!directionalLights[i]->castShadow)
			continue;

		directionalShadowMaps[i].depthTexture->BindTexture(ShadowTextureBinding::Directional);
	}

	m_ShadowMapTextureArray->BindTexture(ShadowTextureBinding::SpotLight);
}

void LightManager::FecthLightInfo()
{
	 if (directionalLights.size() > MaxDirectionalLights)
		Logger::LogWarning("You cannot have more than 1 directional light in the scene");
	
	const size_t nbrOfpointLight = std::clamp<size_t>(pointLights.size(), 0, MaxPointLights);
	const size_t nbrOfSpotLight = std::clamp<size_t>(spotLights.size(), 0, MaxSpotLights);
	const size_t nbrOfDirectionalLight = std::clamp<size_t>(directionalLights.size(), 0, MaxSpotLights);

	m_GpuLightData->nbrOfPointLight = static_cast<uint32_t>(nbrOfpointLight);
	m_GpuLightData->nbrOfSpotLight = static_cast<uint32_t>(nbrOfSpotLight);

	
	for (size_t i = 0; i < nbrOfpointLight; i++)
	{
		const PointLight* pointLight = pointLights[i];
		
		m_GpuLightData->pointLightData[i] =
		{
			.color = static_cast<Vector3>(pointLight->color),
			.intensity = pointLight->intensity,
			.position = static_cast<Vector3>(pointLight->GetEntity()->transform.worldMatrix[3]),
			.radius = 30.f * sqrt(pointLight->intensity),
		};
	}
	for (size_t i = 0 ; i < nbrOfSpotLight ; i++)
	{
		const SpotLight* spotLight = spotLights[i];
		
		m_GpuLightData->spotLightData[i] =
		{
			.color = static_cast<Vector3>(spotLight->color),
			.intensity = spotLight->intensity,
			.position = static_cast<Vector3>(spotLight->GetEntity()->transform.worldMatrix[3]),
			.cutOff = std::cos(spotLight->cutOff * Calc::Deg2Rad),
			.direction = spotLight->GetLightDirection(),
			.outerCutOff = std::cos(spotLight->outerCutOff * Calc::Deg2Rad),
			.isCastingShadow = spotLight->castShadow,
		};
	}
	
	m_GpuLightData->nbrOfSpotLight = static_cast<uint32_t>(nbrOfSpotLight);

	if (nbrOfDirectionalLight != 0)
	for (size_t i = 0 ; i < MaxDirectionalLights ; i++)
	{
		const Vector3 direction = directionalLights[i]->GetLightDirection(); 

		m_GpuLightData->directionalData[i] =
		{
			.color = static_cast<Vector3>(directionalLights[i]->color),
			.intensity = directionalLights[i]->intensity,	
			.direction = { direction.x, direction.y, direction.z },
		};
	}

}

void LightManager::ComputeShadow(const Scene& scene, const Renderer& renderer)
{
	m_ShadowMapShader->Use();
	ComputeShadowDirLight(scene, renderer);
	ComputeShadowSpotLight(scene, renderer);
	m_ShadowMapShader->Unuse();
	ComputeShadowPointLight(scene, renderer);

}

void LightManager::ComputeShadowDirLight(const Scene& scene, const Renderer& renderer)
{
	for (size_t i = 0; i < directionalLights.size(); i++)
	{
		m_GpuLightData->directionalData->isDirlightCastingShadow = directionalLights[i]->castShadow;
		
		if (!directionalLights[i]->castShadow)
			continue;

		const DirectionalShadowMap& shadowMap = directionalShadowMaps[i];
		Camera cam;
		
		cam.isOrthoGraphic = true;
		cam.position = directionalLights[i]->entity->transform.GetPosition();
		cam.LookAt(cam.position + directionalLights[i]->GetLightDirection());
		cam.near = directionalLights[i]->near;
		cam.far = directionalLights[i]->far;
		cam.leftRight = directionalLights[i]->leftRight;
		cam.bottomtop = directionalLights[i]->bottomtop;
		cam.GetVp(shadowMap.depthTexture->GetSize(), &m_GpuLightData->directionalData->lightSpaceMatrix);
		
		m_ShadowFrameBuffer->AttachTexture(*shadowMap.depthTexture, Attachment::Depth, 0);
		RenderPassBeginInfo renderPassBeginInfo =
		{
			.frameBuffer = m_ShadowFrameBuffer,
			.renderAreaOffset = { 0,0 },
			.renderAreaExtent = shadowMap.depthTexture->GetSize() ,
			.clearBufferFlags = BufferFlag::DepthBit,
			.clearColor = Vector4(0.f)
		};
		
		
		renderer.RenderNonShaded(cam, renderPassBeginInfo, m_ShadowRenderPass,m_ShadowMapShader, scene, false);
	}
}

void LightManager::ComputeShadowSpotLight(const Scene& scene, const Renderer& renderer)
{
	for (size_t i = 0; i < spotLights.size(); i++)
	{
		if (!spotLights[i]->castShadow)
			return;
		
		Camera cam;
		cam.position = spotLights[i]->entity->transform.GetPosition();
		cam.LookAt(cam.position + spotLights[i]->GetLightDirection());
		cam.near = spotLights[i]->near;
		cam.far = spotLights[i]->far;

		Matrix matrix;
		cam.GetVp(SpotLightShadowMapSize, &matrix);

		m_GpuLightData->spotLightData[i].lightSpaceMatrix = matrix;

		Rhi::AttachTextureToFrameBufferLayer(m_ShadowFrameBuffer->GetId(), Attachment::Depth, m_ShadowMapTextureArray->GetId(),0,i);
		RenderPassBeginInfo renderPassBeginInfo =
		{
			.frameBuffer = m_ShadowFrameBuffer,
			.renderAreaOffset = { 0,0 },
			.renderAreaExtent = SpotLightShadowMapSize ,
			.clearBufferFlags = BufferFlag::DepthBit,
			.clearColor = Vector4(0.f)
		};
		
		renderer.RenderNonShaded(cam, renderPassBeginInfo, m_ShadowRenderPass,m_ShadowMapShader, scene, false);
	}

}

void LightManager::ComputeShadowPointLight(const Scene& scene, const Renderer& renderer)
{
	std::array<Matrix, 6> vpMatricies;
	Camera cam;

	for (uint32_t i = 0; i < pointLights.size(); i++)
	{
		const Vector3 pos = static_cast<Vector3>(pointLights[i]->entity->transform.worldMatrix[3]);
		Vector3 at;
		Vector3 up;
		
		switch (i)
		{
			case 0:
				at = pos - Vector3::UnitX();
				up = -Vector3::UnitY();
				break;
			case 1:
				at = pos + Vector3::UnitX();
				up = -Vector3::UnitY();
				break;
			case 2:
				at = pos - Vector3::UnitY();
				up = -Vector3::UnitZ();
				break;
			case 3:
				at = pos + Vector3::UnitY();
				up = Vector3::UnitZ();
				break;
			case 4:
				at = pos + Vector3::UnitZ();
				up = -Vector3::UnitY();
				break;
			case 5:
				at = pos - Vector3::UnitZ();
				up = -Vector3::UnitY();
				break;
			
		}

		//renderer.RenderNonShaded()

	}
	

	
}

void LightManager::InitShadow()
{
	m_ShadowMapShader = ResourceManager::Get<Shader>("depth_shader");
	m_ShadowMapShader->SetFaceCullingInfo({ true, CullFace::Front, FrontFace::CCW });
	m_ShadowMapShader->CreateInRhi();

	const TextureCreateInfo textureCreateInfo =
	{
		.size = DirectionalShadowMapSize,
		.filtering = TextureFiltering::Nearest,
		.wrapping = TextureWrapping::TextureWrapping::ClampToBorder,
		.format = TextureFormat::DepthComponent,
		.internalFormat = ShadowDepthTextureInternalFormat,
		.dataType = DataType::Float
	};
	
	for (DirectionalShadowMap& directionalShadowMap : directionalShadowMaps)
	{
		directionalShadowMap.depthTexture = new Texture(textureCreateInfo);
	}

	const TextureCreateInfo spothLightShadowArray =
		{
			.textureType = TextureType::Texture2DArray
			, .mipMaplevel = 1
			, .depth = MaxSpotLights
			, .size = SpotLightShadowMapSize
			, .filtering = TextureFiltering::Nearest
			, .wrapping = TextureWrapping::TextureWrapping::ClampToBorder
			, .format = TextureFormat::DepthComponent
			, .internalFormat =  ShadowDepthTextureInternalFormat
			, .dataType =  DataType::Float
	};

	m_ShadowMapTextureArray = new Texture(spothLightShadowArray);
	
	m_ShadowFrameBuffer = new FrameBuffer;
	Rhi::SetFrameBufferDraw(m_ShadowFrameBuffer->GetId(),false);
	
}
