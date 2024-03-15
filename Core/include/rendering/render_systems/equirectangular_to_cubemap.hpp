#pragma once

#include "core.hpp"
#include "rendering/render_pass.hpp"
#include "resource/cubemap.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"

BEGIN_XNOR_CORE
class EquirectangularToCubeMap
{

public:
    XNOR_ENGINE EquirectangularToCubeMap() = default;
    
    XNOR_ENGINE  ~EquirectangularToCubeMap();

    XNOR_ENGINE void Compute(const Texture& equirectangularMap,const Cubemap& cubemap);

    XNOR_ENGINE void InitResource();
private:
    Pointer<Model> m_Cube;
    FrameBuffer* m_FrameBuffer = nullptr;
    RenderPass m_RenderPass;
    Pointer<Shader> m_Shader;

    XNOR_ENGINE void InitCreateFrameBuffer(Vector2i size);

    Texture* m_TextureDepth = nullptr;
    Texture* m_color = nullptr;
    
};

END_XNOR_CORE
