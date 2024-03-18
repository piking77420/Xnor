#pragma once

#include "core.hpp"
#include "rendering/frame_buffer.hpp"
#include "rendering/render_pass.hpp"
#include "resource/model.hpp"

BEGIN_XNOR_CORE
class BloomPass
{
public:
    DEFAULT_COPY_MOVE_OPERATIONS(BloomPass)

    XNOR_ENGINE BloomPass() = default;

    XNOR_ENGINE ~BloomPass() = default;
    
    XNOR_ENGINE void Init(uint32_t mipChainLength);

    XNOR_ENGINE void Destroy();
    
    XNOR_ENGINE void ComputeBloom(const Texture& textureWithoutBloom);

private:
    FrameBuffer m_FrameBuffer;
    RenderPass m_RenderPass;
    Pointer<Model> m_Quad;
    
    std::vector<Texture*> mMipChain;

    Vector2i m_BloomPassSize;

    void ComputeMiMap(Vector2i Size);
    
};

END_XNOR_CORE
