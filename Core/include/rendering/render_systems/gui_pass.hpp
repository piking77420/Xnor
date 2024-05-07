#pragma once
#include "core.hpp"
#include "resource/shader.hpp"
#include "scene/scene.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_CORE
class GuiPass
{
public:
    XNOR_ENGINE GuiPass() = default;

    XNOR_ENGINE ~GuiPass() = default;
    
    XNOR_ENGINE void RenderGui(const Scene& scene);
    
private:
    Pointer<Shader> GuiShader;

    XNOR_ENGINE void RenderButton();

    XNOR_ENGINE void RenderImage();
};

END_XNOR_CORE