#pragma once

#include "core.hpp"
#include "rendering/render_pass.hpp"
#include "resource/cubemap.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"
#include "scene/scene.hpp"

BEGIN_XNOR_CORE
class XNOR_ENGINE EquirectangularToCubeMap
{

public:
    EquirectangularToCubeMap();
    
    ~EquirectangularToCubeMap();
    
    void Compute(const Texture& equirectangularMap,const Cubemap& cubemap);

private:
    Pointer<Model> m_Cube;
    FrameBuffer* m_FrameBuffer = nullptr;
    RenderPass m_RenderPass;
    Pointer<Shader> m_Shader;

    void InitCreateFrameBuffer(Vector2i size);

    Texture* m_TextureDepth = nullptr;
    
};

END_XNOR_CORE
