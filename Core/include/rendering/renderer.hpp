#pragma once

#include "core.hpp"
#include "Maths/vector4.hpp"
#include "renderer_context.hpp"
#include "file/file_manager.hpp"
#include "scene/scene.hpp"
#include "rendering/rhi.hpp"
#include "resource/model.hpp"

BEGIN_XNOR_CORE

class Renderer
{
public:
    Vector4 clearColor;

    XNOR_ENGINE explicit Renderer();

    XNOR_ENGINE ~Renderer();
    
    XNOR_ENGINE void RenderScene(const Scene& scene, const RendererContext& rendererContext) const;

    XNOR_ENGINE void CompileShader();
    
    XNOR_ENGINE void OnResizeWindow();
    
    XNOR_ENGINE void SwapBuffers();

    XNOR_ENGINE void PrepareRendering(vec2i windowSize);

private:

    FrameBuffer* m_RenderBuffer = nullptr;

    Texture m_ColorAttachment;
    Texture m_DepthAttachment;
    
    RHI m_Rhi;

    // Basics Shader
    Pointer<File> m_VertexPath;
    Pointer<File> m_FragmentPath;
    Pointer<Shader> m_BasicShader;
    // DrawTextureToScreen
    Pointer<File> m_VertexDrawTextureToScreenPath;
    Pointer<File> DrawTextureToScreen;
    Pointer<Shader> m_DrawTextureToScreenShader;

    Pointer<Model> m_Quad;

    
    void UpdateLight(const Scene& scene, const RendererContext& rendererContext) const;

    void DrawMeshRenders(const Scene& scene, const RendererContext& rendererContext) const;

    void SetViewPort(const Camera& camera);
};

END_XNOR_CORE
