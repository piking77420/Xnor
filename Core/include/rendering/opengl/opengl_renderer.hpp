#pragma once

#include "rendering/renderer.hpp"
#include "Maths/vector4.hpp"

class XNOR_ENGINE OpenglRenderer : public Renderer
{
public:

    OpenglRenderer();
    
    void SetClearColor(const Vector4& color);

    void ClearColorAndDepth();

};
