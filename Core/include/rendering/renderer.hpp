#pragma once

#include <Maths/vector4.hpp>

#include "core.hpp"
#include "frustum.hpp"
#include "material.hpp"
#include "viewport.hpp"
#include "render_systems/animation_render.hpp"
#include "render_systems/gui_pass.hpp"
#include "render_systems/light_manager.hpp"
#include "render_systems/skybox_renderer.hpp"
#include "resource/model.hpp"
#include "world/world.hpp"
#include "render_systems/post_process_pass.hpp"

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
    
    XNOR_ENGINE Renderer() = default;
    XNOR_ENGINE ~Renderer() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Renderer)

    /// @brief Initializes the renderer
    XNOR_ENGINE void Initialize();

    /// @brief Renders a scene on a viewport
    /// @param viewport Viewport target
    /// @param scene Scene to render
    XNOR_ENGINE void RenderViewport(const Viewport& viewport, const Scene& scene);

    /// @brief Renders a scene without shading , calling begin and endfrane 
    /// @param camera Camera
    /// @param renderPassBeginInfo Render pass begin info
    /// @param renderPass Render pass
    /// @param shaderToUse Shader to use
    /// @param scene Scene to render
    /// @param drawEditorUi Whether to draw the editor only UI
    XNOR_ENGINE void ZPass(const Scene& scene,const Camera& camera, const RenderPassBeginInfo& renderPassBeginInfo, const RenderPass& renderPass,
        const Pointer<Shader>& shaderToUse, bool_t drawEditorUi);

    /// @brief Renders a scene without shading
    /// @param camera Camera
    /// @param renderPassBeginInfo Render pass begin info
    /// @param renderPass Render pass
    /// @param shaderToUse Shader to use
    /// @param scene Scene to render
    /// @param drawEditorUi Whether to draw the editor only UI
    XNOR_ENGINE void RenderNonShadedPass(const Scene& scene, const Camera& camera, const RenderPassBeginInfo& renderPassBeginInfo, const RenderPass& renderPass, const
                                         Pointer<Shader>& shaderToUse, bool_t drawEditorUi);
    
    /// @brief Swaps the front and back buffer.
    XNOR_ENGINE void SwapBuffers() const;

    XNOR_ENGINE void RenderMenu(const Scene& scene);
private:
     LightManager m_LightManager;
     SkyboxRenderer m_SkyboxRenderer;
     PostProcessPass m_PostProcessPass;
     AnimationRender m_AnimationRender;
     GuiPass m_GuiPass;
    
     std::vector<const MeshRenderer*> m_MeshRenderers;
     mutable Frustum m_Frustum;
    
     Pointer<Shader> m_GBufferShader;
     Pointer<Shader> m_GBufferShaderLit;
    
     Pointer<Shader> m_Forward;
     Pointer<Shader> m_DrawTextureToScreenShader;
     Pointer<Shader> m_GizmoShader;
    
     Pointer<Mesh> m_Quad;
     Pointer<Mesh> m_Cube;

    /// @brief Update All the infos of the current frame lights, animation, etc...
    /// @param scene The scene
    XNOR_ENGINE void BeginFrame(const Scene& scene, const Viewport& viewport);

    /// @brief stuff made at the end of the frame
    /// @param scene The scene
    XNOR_ENGINE void EndFrame(const Scene& scene);
    
    
    XNOR_ENGINE void BindCamera(const Camera& camera, Vector2i screenSize) const;
    
    XNOR_ENGINE void InitResources();
    
    XNOR_ENGINE void DrawMeshRendersByType(MaterialType materialType , const Scene& scene) const;
    
    XNOR_ENGINE void DrawAllMeshRendersNonShaded(const Camera& camera, const Scene& scene) const;

    XNOR_ENGINE void DeferedRenderring(const Camera& camera, const Scene& scene, const ViewportData&
                                       viewportData, const Vector2i viewportSize) const;
    
    XNOR_ENGINE void ForwardPass(const std::vector<const MeshRenderer*>& meshRenderers, const Scene& scene,
        const Viewport& viewport, Vector2i viewportSize, bool_t isEditor) const;
    
    XNOR_ENGINE void DrawAabb(const std::vector<const MeshRenderer*>& meshRenderers) const;

    XNOR_ENGINE void PrepareOctree(const Scene& scene);


};

END_XNOR_CORE
