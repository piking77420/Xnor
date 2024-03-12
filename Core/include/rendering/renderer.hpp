#pragma once

#include <Maths/vector4.hpp>

#include "core.hpp"
#include "render_systems/light_manager.hpp"
#include "viewport.hpp"
#include "render_systems/skybox_renderer.hpp"
#include "render_systems/tone_mapping.hpp"
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
    
    /// @brief Update All the infos of the current frame lights, animation, etc...
    /// @param scene The current world's scene of the app
    XNOR_ENGINE void BeginFrame(const Scene& scene);

    /// @brief stuff made at the end of the frame
    /// @param world The current world's scene of the app
    XNOR_ENGINE void EndFrame(const Scene& scene);

    /// @brief Renders the current scene
    /// @param rendererContext Renderer context
    XNOR_ENGINE void RenderViewport(const Viewport& viewport, Scene& scene) const;

    XNOR_ENGINE void RenderNonShaded(const Camera& camera,const RenderPassBeginInfo& renderPassBeginInfo , const RenderPass& renderPass, const Pointer<Shader>& shadertoUse
        ,const Scene& scene, bool_t drawEditorUi) const;

    
    /// @brief Swaps the front and back buffer
    XNOR_ENGINE void SwapBuffers();
private:
    LightManager m_LightManager;
    ToneMapping m_ToneMapping;
    SkyboxRenderer m_SkyboxRenderer;
    
    
    Pointer<Shader> m_GBufferShader;
    Pointer<Shader> m_GBufferShaderLit;
    
    Pointer<Shader> m_Forward;
    Pointer<Shader> m_DrawTextureToScreenShader;
    Pointer<Shader> m_GizmoShader;
    
    Pointer<Model> m_Quad;
    Pointer<Model> m_Cube;
    
    XNOR_ENGINE void BindCamera(const Camera& camera,const Vector2i screenSize) const;
    
    XNOR_ENGINE void InitResources();
    
    XNOR_ENGINE void DrawMeshRendersByType(const std::vector<const MeshRenderer*>& meshRenderers, MaterialType materialtype) const;
    
    XNOR_ENGINE void DrawAllMeshRenders(const std::vector<const MeshRenderer*>& meshRenderers,const Scene& scene) const;
    
    XNOR_ENGINE void DefferedRendering(const std::vector<const MeshRenderer*>& meshRenderers, const Skybox& skybox, const ViewportData& viewportData, Vector2i viewportSize) const;
    
    XNOR_ENGINE void ForwardPass(const std::vector<const MeshRenderer*>& meshRenderers,Skybox& skybox,
        const Viewport& Viewport, Vector2i viewportSize, bool isEditor) const;
    
    XNOR_ENGINE void DrawAabb(const std::vector<const MeshRenderer*>& meshRenderers) const;
};

END_XNOR_CORE
