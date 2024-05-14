#pragma once

#include "core.hpp"
#include "rendering/rhi_typedef.hpp"
#include "resource/shader.hpp"
#include "scene/component/skinned_mesh_renderer.hpp"

BEGIN_XNOR_CORE

class Scene;
class Renderer;

class AnimationRender
{
public:
    XNOR_ENGINE AnimationRender();

    XNOR_ENGINE ~AnimationRender();

    DEFAULT_COPY_MOVE_OPERATIONS(AnimationRender)

    XNOR_ENGINE void InitResources();

    XNOR_ENGINE void BeginFrame(const Scene& scene, const Renderer& renderer);

    // Render All the animated mesh only work on Deferred Rendering
    XNOR_ENGINE void RenderAnimation() const;

    XNOR_ENGINE void EndFrame();

private:
    SkinnedMeshGpuData* m_SkinnedMeshGpuData = nullptr;

    Pointer<Shader> m_SkinnedShader;

    std::vector<const SkinnedMeshRenderer*> m_SkinnedRender;
    
};


END_XNOR_CORE
