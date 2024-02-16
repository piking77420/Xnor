#pragma once

#include "core.hpp"
#include "Maths/vector4.hpp"
#include "renderer_context.hpp"
#include "scene/scene.hpp"
#include "rendering/rhi.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE Renderer
{
public:
    Vector4 clearColor = 0.5f;

    Renderer();

    ~Renderer() = default;
    
    void RenderScene(const Scene& scene, const RendererContext& rendererContext) const;
private:

    RHI m_Rhi;
};

END_XNOR_CORE
