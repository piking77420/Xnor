#pragma once

#include "core.hpp"
#include "Maths/vector4.hpp"
#include "renderer_context.hpp"
#include "file/file_manager.hpp"
#include "scene/scene.hpp"
#include "rendering/rhi.hpp"

BEGIN_XNOR_CORE

class Renderer
{
public:
    Vector4 clearColor;

    XNOR_ENGINE Renderer();

    XNOR_ENGINE  ~Renderer() = default;
    
    XNOR_ENGINE void RenderScene(const Scene& scene, const RendererContext& rendererContext) const;
private:
    
    Pointer<File> vertexPath;
    Pointer<File> fragmentPath;
    Shader* basicShader;
    
    RHI m_Rhi;
};

END_XNOR_CORE
