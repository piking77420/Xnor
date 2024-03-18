#pragma once

#include "core.hpp"
#include "rendering/frame_buffer.hpp"
#include "rendering/render_pass.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"

BEGIN_XNOR_CORE
class BloomPass
{
public:
    DEFAULT_COPY_MOVE_OPERATIONS(BloomPass)

    XNOR_ENGINE BloomPass() = default;

    XNOR_ENGINE ~BloomPass() = default;
    
    XNOR_ENGINE void Init(uint32_t bloomMips);
    
    XNOR_ENGINE void ComputeBloom(const Texture& textureWithoutBloom);

    XNOR_ENGINE Texture* GetBloomTexture() const;

private:
    struct BloomMip
    {
        Texture* texture;
        Vector2 sizef;
    };
    
    FrameBuffer* m_FrameBuffer = nullptr;
    RenderPass m_RenderPass;
    static inline Pointer<Model> m_Quad = nullptr;
    static inline Pointer<Shader> m_DownSample = nullptr;
    static inline Pointer<Shader> m_UpSample = nullptr;

    std::vector<BloomMip> m_MipChain;

    uint32_t m_BloomMips{};

    float_t filterRadius = 0.005f;

    XNOR_ENGINE void UpSampling();
    
    XNOR_ENGINE void DownSampling(const Texture& textureWithoutBloom);

    XNOR_ENGINE void HandleBlooMip(Vector2i currentViewPortSize);
    

};

END_XNOR_CORE
