#pragma once

#include "rendering/renderer.hpp"

class XNOR_ENGINE OpenglRenderer : public Renderer
{
public:

    OpenglRenderer();
    
    void SetClearColor(const Vector4& color) override;

    void ClearColorAndDepth() override;

};
