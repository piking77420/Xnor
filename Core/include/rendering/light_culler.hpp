#pragma once

#include <vector>

#include "camera.hpp"
#include "core.hpp"
#include "light/directional_light.hpp"
#include "light/point_light.hpp"
#include "light/spot_light.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"

BEGIN_XNOR_CORE
class LightCuller
{
public:

    XNOR_ENGINE LightCuller() = default;

    XNOR_ENGINE ~LightCuller() = default;
    
    DEFAULT_COPY_MOVE_OPERATIONS(LightCuller)

    XNOR_ENGINE void InitResources();
    
    XNOR_ENGINE void UpdateLight(const std::vector<const PointLight*>& pointLightComponents,
        const std::vector<const SpotLight*>& spotLightsComponents,
        const std::vector<const DirectionalLight*>& directionalComponent) const;

    XNOR_ENGINE void DrawLightGizmo(const std::vector<const PointLight*>& pointLightComponents,
        const std::vector<const SpotLight*>& spotLightsComponents,
        const std::vector<const DirectionalLight*>& directionalComponent,const Camera& camera) const;
    
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

    Pointer<Shader> m_editorUI;
    Pointer<Model> m_Quad;

};

END_XNOR_CORE