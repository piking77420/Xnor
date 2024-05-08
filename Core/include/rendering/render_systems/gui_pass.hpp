#pragma once
#include "core.hpp"
#include "resource/shader.hpp"
#include "scene/scene.hpp"
#include "scene/component/button.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_CORE
class GuiPass
{
public:
    XNOR_ENGINE GuiPass() = default;

    XNOR_ENGINE ~GuiPass() = default;
    
    XNOR_ENGINE void RenderGui(const Scene& scene, Vector2i viewPortSize) const;

    XNOR_ENGINE void Init();
    
private:
    Pointer<Shader> m_GuiShader;

    Pointer<Model> m_Quad;

    mutable std::vector<const Button*> m_Buttons;

    mutable std::vector<const Image*> m_Images;
    
    XNOR_ENGINE void RenderImage(const Vector2i viewPortSize) const;
};

END_XNOR_CORE