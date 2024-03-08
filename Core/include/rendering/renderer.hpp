#pragma once

#include <Maths/vector4.hpp>

#include "core.hpp"
#include "light_manager.hpp"
#include "renderer_context.hpp"
#include "skybox_renderer.hpp"
#include "tone_mapping.hpp"
#include "rendering/render_pass.hpp"
#include "resource/model.hpp"
#include "world/world.hpp"

/// @file renderer.hpp
/// @brief Defines the XnorCore::Renderer class
BEGIN_XNOR_CORE

class DirectionalLight;
class SpotLight;
class PointLight;
class MeshRenderer;

/// @brief Handles rendering a scene given a RendererContext
class Renderer
{
public:
    /// @brief Clear color
    Vector4 clearColor = Vector4(0.f);

    /// @brief Default constructs a Renderer.
    XNOR_ENGINE Renderer() = default;

    /// @brief Default destructs the Renderer.
    XNOR_ENGINE ~Renderer() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Renderer)

    /// @brief Initializes the renderer
    XNOR_ENGINE void Initialize();

    /// @brief Shutdowns the renderer
    XNOR_ENGINE void Shutdown();

    /// @brief Renders the current scene
    /// @param rendererContext Renderer context
    XNOR_ENGINE void RenderScene(const RendererContext& rendererContext) const;

    /// @brief Re-compiles the internal shaders
    XNOR_ENGINE void CompileShader(); 

    /// @brief Swaps the front and back buffer
    XNOR_ENGINE void SwapBuffers();

    /// @brief Handles a window resize event
    /// @param windowSize Window size
    XNOR_ENGINE void OnResize(Vector2i windowSize);

    /// @brief Prepares rendering
    /// @param windowSize Window size
    XNOR_ENGINE void PrepareRendering(Vector2i windowSize);

private:
    LightManager m_LightManager;
    
    
    FrameBuffer* m_GframeBuffer = nullptr;
    // Deferred attachment GBuffers
    Texture* m_PositionAtttachment = nullptr;
    Texture* m_NormalAttachement = nullptr;
    Texture* m_AlbedoAttachment = nullptr;

    
    Texture* m_DepthGbufferAtttachment = nullptr;
    RenderPass m_GbufferPass;
    
    FrameBuffer* m_RenderBuffer = nullptr;
    // Forward Attachment
    Texture* m_ColorAttachment = nullptr;
    Texture* m_DepthAttachment = nullptr;

    Texture* m_Stencsil = nullptr;
    
    Pointer<Shader> m_GBufferShader;
    Pointer<Shader> m_GBufferShaderLit;
    
    Pointer<Shader> m_Forward;
    Pointer<Shader> m_DrawTextureToScreenShader;
    Pointer<Shader> m_GizmoShader;
    
    Pointer<Model> m_Quad;
    Pointer<Model> m_Cube;

    ToneMapping m_ToneMapping;
    SkyboxRenderer m_SkyboxRenderer;

    XNOR_ENGINE void DefferedRendering(const std::vector<const MeshRenderer*>& meshrenderers,const RendererContext* rendererContext) const;

    XNOR_ENGINE void ForwardRendering(const std::vector<const MeshRenderer*>& meshrenderers,const RendererContext* rendererContext) const;
    
    XNOR_ENGINE void InitResources();
    
    XNOR_ENGINE void InitDefferedRenderingAttachment(Vector2i windowSize);
    
    XNOR_ENGINE void InitForwardRenderingAttachment(Vector2i windowSize);
    
    XNOR_ENGINE void DestroyAttachment() const;

    XNOR_ENGINE void DrawMeshRendersByType(const std::vector<const MeshRenderer*>& meshRenderers,MaterialType materialtype) const;
    
    XNOR_ENGINE void DrawAabb(const std::vector<const MeshRenderer*>& meshRenderers) const;

    XNOR_ENGINE void RenderAllMeshes(const std::vector<const MeshRenderer*>& meshRenderers) const;

    XNOR_ENGINE uint32_t FetchDrawIndexToGpu(uint32_t meshRenderIndex) const;

    //XNOR_ENGINE uint32_t GetMeshRenderIndexFromGpu(uint32_t meshRenderIndex) const;
    
};

END_XNOR_CORE
