#pragma once

#include "core.hpp"
#include "rendering/render_pass.hpp"
#include "resource/cubemap.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"
#include "scene/scene.hpp"

BEGIN_XNOR_CORE
    class EquirectangularToCubeMap
{

public:
    STATIC_CLASS(EquirectangularToCubeMap)

    XNOR_ENGINE static void Initalize();
    
    XNOR_ENGINE static void Shutdown();

    XNOR_ENGINE static void Compute(const Texture& equirectangularMap,const Cubemap& cubemap);

private:
    static inline Pointer<Model> m_Cube;
    
    static inline FrameBuffer* m_FrameBuffer = nullptr;
    static inline RenderPass m_RenderPass;
    static inline Pointer<Shader> m_Shader;
};

END_XNOR_CORE