﻿#pragma once

#include "core.hpp"
#include "rendering/light/directional_light.hpp"
#include "rendering/light/point_light.hpp"
#include "rendering/light/spot_light.hpp"
#include "rendering/camera.hpp"
#include "rendering/frame_buffer.hpp"
#include "rendering/light/directional_shadowmap.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"
#include "scene/scene.hpp"

/// @file light_manager.hpp
/// @brief Defines the XnorCore::LightManager class.

BEGIN_XNOR_CORE
class Renderer;

    /// @brief Handles rendering lights and their gizmos
class LightManager
{
private:
    static constexpr Vector2i DirectionalShadowMapSize = { 1024, 1024 };
    static constexpr Vector2i SpotLightShadowMapSize = { 1024, 1024 };
    static constexpr Vector2i PointLightLightShadowMapSize = { 1024, 1024 };

    static constexpr TextureInternalFormat::TextureInternalFormat ShadowDepthTextureInternalFormat = TextureInternalFormat::DepthComponent32F;
    
public:
    XNOR_ENGINE LightManager() = default;

    XNOR_ENGINE ~LightManager();
    
    DEFAULT_COPY_MOVE_OPERATIONS(LightManager)

    /// @brief Initializes the light manager resources
    XNOR_ENGINE void InitResources();

    /// @brief Computes the internal lights to send to the GPU
    XNOR_ENGINE void BeginFrame(const Scene& scene, const Renderer& renderer);
    
    /// @brief End frame
    XNOR_ENGINE void EndFrame(const Scene& scene);

    /// @brief Draws the light gizmos
    XNOR_ENGINE void DrawLightGizmo(const Camera& camera, const Scene& scene);

    XNOR_ENGINE void DrawLightGizmoWithShader(const Camera& camera, const Scene& scene,const Pointer<Shader>& shader) const;
    
    XNOR_ENGINE void BindShadowMap() const;


    std::vector<const PointLight*> pointLights;
    std::vector<const SpotLight*> spotLights;
    std::vector<const DirectionalLight*> directionalLights;
    std::array<DirectionalShadowMap,MaxDirectionalLights> directionalShadowMaps;

private:
    enum class RenderingLight
    {
        PointLight,
        SpothLight,
        DirLight
    };

    struct GizmoLight
    {
        Vector3 pos;
        const Light* light = nullptr;
        RenderingLight type;
    };

    GpuLightData* m_GpuLightData = nullptr;

    Pointer<Texture> m_PointLightTexture;
    Pointer<Texture> m_DirLightTexture;
    Pointer<Texture> m_SpotLightTexture;
    
    Pointer<Shader> m_ShadowMapShader;
    Pointer<Shader> m_ShadowMapShaderPointLight;

    float_t m_ScaleFactor = 2.f;
    float_t m_MinDistance = 1.f;
    float_t m_MinScalarFactor = 10.f;
    float_t m_MaxScalarFactor = m_ScaleFactor;
    Pointer<Shader> m_EditorUi;
    Pointer<Model> m_Quad;
    
    RenderPass m_ShadowRenderPass;
    FrameBuffer* m_ShadowFrameBuffer {nullptr};
    
    Texture* m_SpotLightShadowMapTextureArray = nullptr;
    
    Texture* m_PointLightShadowMapCubemapArrayPixelDistance = nullptr;
    Texture* m_DepthBufferForPointLightPass = nullptr;

    
    XNOR_ENGINE void FecthLightInfo();

    XNOR_ENGINE void ComputeShadow(const Scene& scene, const Renderer& renderer);

    XNOR_ENGINE void ComputeShadowDirLight(const Scene& scene, const Renderer& renderer);

    XNOR_ENGINE void ComputeShadowSpotLight(const Scene& scene, const Renderer& renderer);

    XNOR_ENGINE void ComputeShadowPointLight(const Scene& scene, const Renderer& renderer);
    
    XNOR_ENGINE void InitShadow();
};

END_XNOR_CORE
