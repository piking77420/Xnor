#pragma once

#include "core.hpp"
#include "renderer_context.hpp"
#include "file/file_manager.hpp"
#include "Maths/vector4.hpp"
#include "rendering/rhi.hpp"
#include "resource/model.hpp"
#include "scene/scene.hpp"

BEGIN_XNOR_CORE

class Renderer
{
public:
    Vector4 clearColor;

    XNOR_ENGINE explicit Renderer();
    
    XNOR_ENGINE ~Renderer();
    
    XNOR_ENGINE void Initialize();
    
    XNOR_ENGINE void RenderScene(const Scene& scene, const RendererContext& rendererContext) const;

    XNOR_ENGINE void CompileShader(); 
    
    XNOR_ENGINE void OnResizeWindow();
    
    XNOR_ENGINE void SwapBuffers();

    XNOR_ENGINE void PrepareRendering(vec2i windowSize);

private:

    FrameBuffer* m_RenderBuffer = nullptr;
    Texture* m_ColorAttachment = nullptr;
    Texture* m_DepthAttachment = nullptr;
    
    RHI m_Rhi;

    // Basics Shader
    Pointer<File> m_VertexPath;
    Pointer<File> m_FragmentPath;
    Pointer<Shader> m_BasicShader;
    // DrawTextureToScreen
    Pointer<File> m_VertexDrawTextureToScreenPath;
    Pointer<File> m_DrawTextureToScreen;
    Pointer<Shader> m_DrawTextureToScreenShader;

    //
    Pointer<Shader> m_GizmoShader;

    
    Pointer<Model> m_Quad;
    Pointer<Model> m_Cube;

    
    XNOR_ENGINE void UpdateLight(const Scene& scene, const RendererContext& rendererContext) const;

    XNOR_ENGINE void DrawMeshRenders(const Scene& scene, const RendererContext& rendererContext) const;

    XNOR_ENGINE void SetViewport(const Camera& camera);
};

END_XNOR_CORE
