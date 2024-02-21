#pragma once

#include "core.hpp"
#include "renderer_context.hpp"
#include "file/file_manager.hpp"
#include "Maths/vector4.hpp"
#include "rendering/rhi.hpp"
#include "scene/scene.hpp"

BEGIN_XNOR_CORE

class Renderer
{
public:
    Vector4 clearColor;

    XNOR_ENGINE explicit Renderer();
    
    XNOR_ENGINE void RenderScene(const Scene& scene, const RendererContext& rendererContext) const;

    XNOR_ENGINE void CompileShader();

private:
    RHI m_Rhi;
    
    Pointer<File> m_VertexPath;
    
    Pointer<File> m_FragmentPath;
    
    Pointer<Shader> m_BasicShader;
    
    void UpdateLight(const Scene& scene, const RendererContext& rendererContext) const;

    void DrawMeshRenders(const Scene& scene, const RendererContext& rendererContext) const;

    void SetViewport(const Camera& camera);
};

END_XNOR_CORE
