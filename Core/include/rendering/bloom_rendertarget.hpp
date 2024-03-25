#pragma once

#include "core.hpp"
#include "render_pass.hpp"
#include "resource/texture.hpp"
#include "frame_buffer.hpp"

BEGIN_XNOR_CORE

class BloomRenderTarget
{
private:
    static inline constexpr TextureInternalFormat::TextureInternalFormat BloomTextureFormat = TextureInternalFormat::TextureInternalFormat::Rgba32F;
    static inline constexpr uint32_t BloomMipNumber = 5;
public:
    struct BloomMip
    {
        XnorCore::Texture* texture {nullptr};
        Vector2 sizef;
    };

    std::vector<BloomMip> mipChain;
    Texture* thresholdTexture = nullptr;

    FrameBuffer* frameBuffer = nullptr;
    RenderPass renderPass;
    
    DEFAULT_COPY_MOVE_OPERATIONS(BloomRenderTarget)

    XNOR_ENGINE BloomRenderTarget();

    XNOR_ENGINE ~BloomRenderTarget();

    XNOR_ENGINE void Initialize(Vector2i viewportSize);

    XNOR_ENGINE void Destroy();

    XNOR_ENGINE void Resize(Vector2i viewportSize);

    XNOR_ENGINE Texture* GetBloomedTexture() const;

    XNOR_ENGINE void CreateBloomMip(Vector2i viewportSize);

};

END_XNOR_CORE