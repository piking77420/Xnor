#pragma once

#include "core.hpp"
#include "Maths/vector4.hpp"

class XNOR_ENGINE Renderer
{
public:
    virtual ~Renderer() = default;

    virtual void SetClearColor(const Vector4& color) const = 0;

    virtual void ClearColorAndDepth() = 0;

protected:

};

