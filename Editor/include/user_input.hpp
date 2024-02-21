#pragma once

#include "definitions.hpp"
#include "rendering/renderer_context.hpp"

BEGIN_XNOR_EDITOR
class UserInput
{
public:
    
    XnorCore::RendererContext userRenderContext;

    UserInput();
    
    ~UserInput() = default;
    
    void UpdateUserInut(bool isPlaying);
    
private:
    void CameraMovement();
    
};

END_XNOR_EDITOR
