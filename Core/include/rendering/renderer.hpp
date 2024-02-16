#pragma once

#include "core.hpp"
#include "Maths/vector4.hpp"
#include "renderer_context.hpp"
#include "scene/scene.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE Renderer
{
public:

    Renderer();

    ~Renderer() = default;

    void SetClearColor(const Vector4& color) const;

    void ClearColorAndDepth();

    void RenderScene(const Scene& scene, const RendererContext& rendererContext) const;

};

END_XNOR_CORE
