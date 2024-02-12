#pragma once

#include "rendering/renderer.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE OpenGlRenderer : public Renderer
{
public:
    OpenGlRenderer();
    
    void SetClearColor(const Vector4& color) const override;

    void ClearColorAndDepth() override;
};

END_XNOR_CORE
