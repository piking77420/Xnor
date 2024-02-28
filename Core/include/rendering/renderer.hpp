#pragma once

#include "core.hpp"
#include "renderer_context.hpp"
#include "file/file_manager.hpp"
#include "Maths/vector4.hpp"
#include "rendering/rhi.hpp"
#include "resource/model.hpp"
#include "scene/scene.hpp"


BEGIN_XNOR_CORE
    class DirectionalLight;
    class SpotLight;
    class PointLight;
    class MeshRenderer;

class Renderer
{
public:
    Vector4 clearColor;

    XNOR_ENGINE explicit Renderer();
    
    XNOR_ENGINE ~Renderer();
    
    XNOR_ENGINE void Initialize();
    
    XNOR_ENGINE void Shutdown();
    
    XNOR_ENGINE void RenderScene(const Scene& scene, const RendererContext& rendererContext) const;

    XNOR_ENGINE void CompileShader(); 
    
    XNOR_ENGINE void SwapBuffers();

    XNOR_ENGINE void PrepareRendering(vec2i windowSize);

private:
    enum class RenderingLight
    {
        PointLight,
        SpothLight,
        DirLight
    };

    struct GizmoLight
    {
        Vector3 pos;
        RenderingLight type;
    };
    
    FrameBuffer* m_GframeBuffer = nullptr;
    // Deffered attachment GBuffers
    Texture* m_PositionAtttachment = nullptr;
    Texture* m_NormalAttachement = nullptr;
    Texture* m_AlbedoAtttachment = nullptr;
    Texture* m_DepthGbufferAtttachment = nullptr;
    RenderPass m_GbufferPass;
    

    
    FrameBuffer* m_RenderBuffer = nullptr;
    // Forward Attahcment
    Texture* m_ColorAttachment = nullptr;
    Texture* m_DepthAttachment = nullptr;

    Pointer<Shader> m_gBufferShader;
    Pointer<Shader> m_gBufferShaderLit;
    
    Pointer<Shader> m_BasicShader;
    Pointer<Shader> m_DrawTextureToScreenShader;
    Pointer<Shader> m_GizmoShader;
    
    Pointer<Model> m_Quad;
    Pointer<Model> m_Cube;

  
    
    XNOR_ENGINE void UpdateLight(const std::vector<const PointLight*>& pointLightComponents,
    const std::vector<const SpotLight*>& spotLightsComponents,
    const std::vector<const DirectionalLight*>& directionalComponent) const;

    XNOR_ENGINE void DrawLightGizmo(const std::vector<const PointLight*>& pointLightComponents,
     const std::vector<const SpotLight*>& spotLightsComponents,
    const std::vector<const DirectionalLight*>& directionalComponent,const Camera& camera) const;

    XNOR_ENGINE void DrawMeshRendersByType(const std::vector<const MeshRenderer*>& meshRenderers,MaterialType materialtype) const;
    
    XNOR_ENGINE void SetViewport(const Camera& camera);

    XNOR_ENGINE void InitDefferedRendering(vec2i windowSize);
        
    XNOR_ENGINE void InitForwardRendering(vec2i windowSize);

    XNOR_ENGINE void DrawAABB(const std::vector<const MeshRenderer*>& meshRenderers) const;

    XNOR_ENGINE void RenderAllMeshes(const std::vector<const MeshRenderer*>& meshRenderers);

    
    XNOR_ENGINE void ShadowPathSpotLight(const std::vector<const SpotLight*>& spotLights);

    
};

END_XNOR_CORE
