#pragma once

#include "core.hpp"
#include "rendering/rhi_typedef.hpp"
#include "resource/shader.hpp"
#include "scene/component/skinned_mesh_render.hpp"

BEGIN_XNOR_CORE
    class Scene;
    class Renderer;

class AnimationRender
{
public:

    XNOR_ENGINE AnimationRender();

    XNOR_ENGINE ~AnimationRender();

    XNOR_ENGINE void InitResources();

    XNOR_ENGINE void BeginFrame(const Scene& scene, const Renderer& renderer);

    // Render All the animated mesh only work on Deffered Rendering
    XNOR_ENGINE void RenderAnimation() const;

    XNOR_ENGINE void EndFrame();

private:
    SkinnedMeshGPUData* m_SkinnedMeshGPUData;

    Pointer<Shader> m_SkinnedShader;

    std::vector<const SkinnedMeshRender*> m_SkinnedRender;
};


END_XNOR_CORE
