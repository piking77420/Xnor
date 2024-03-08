#pragma once

#include <vector>

#include "core.hpp"
#include "light/directional_light.hpp"
#include "light/point_light.hpp"
#include "light/spot_light.hpp"
#include "rendering/camera.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"

/// @file light_manager.hpp
/// @brief Defines the XnorCore::LightManager class.

BEGIN_XNOR_CORE

/// @brief Handles rendering lights and their gizmos
class LightManager
{
public:
    XNOR_ENGINE LightManager() = default;

    XNOR_ENGINE ~LightManager() = default;
    
    DEFAULT_COPY_MOVE_OPERATIONS(LightManager)

    /// @brief Initializes the light manager resources
    XNOR_ENGINE void InitResources();

    /// @brief Computes the internal lights to send to the GPU
    XNOR_ENGINE void UpdateLight(const std::vector<const PointLight*>& pointLightComponents,
        const std::vector<const SpotLight*>& spotLightsComponents,
        const std::vector<const DirectionalLight*>& directionalComponent) const;

    /// @brief Draws the light gizmos
    XNOR_ENGINE void DrawLightGizmo(const std::vector<const PointLight*>& pointLightComponents,
        const std::vector<const SpotLight*>& spotLightsComponents,
        const std::vector<const DirectionalLight*>& directionalComponent, const Camera& camera) const;
    
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
        RenderingLight type;
    };

    Pointer<Texture> m_PointLightTexture;
    Pointer<Texture> m_DirLightTexture;
    Pointer<Texture> m_SpotLightTexture;

    Pointer<Shader> m_EditorUi;
    Pointer<Model> m_Quad;

    float_t m_ScaleFactor = 2.f;
    float_t m_MinDistance = 1.f;
    float_t m_MinScalarFactor = 10.f;
    float_t m_MaxScalarFactor = m_ScaleFactor;
};

END_XNOR_CORE
