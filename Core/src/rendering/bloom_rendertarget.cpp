#include "rendering/bloom_rendertarget.hpp"

#include "window.hpp"

using namespace XnorCore;

BloomRenderTarget::BloomRenderTarget()
{
    Initialize(Window::GetSize());
}

BloomRenderTarget::~BloomRenderTarget()
{
    Destroy();
}

void BloomRenderTarget::Initialize(const Vector2i viewportSize)
{
    CreateBloomMip(viewportSize);
    thresholdTexture = new Texture(TextureInternalFormat::Rgba32F, viewportSize);
    frameBuffer = new FrameBuffer();
}

void BloomRenderTarget::Destroy()
{
    for (const BloomMip& mip : mipChain)
        delete mip.texture;
    mipChain.clear();

    delete thresholdTexture;
    delete frameBuffer;
}

void BloomRenderTarget::Resize(const Vector2i viewportSize)
{
    Destroy();
    Initialize(viewportSize);
}

Texture* BloomRenderTarget::GetBloomedTexture() const
{
    return mipChain[0].texture;
}

void BloomRenderTarget::CreateBloomMip(const Vector2i viewportSize)
{
    mipChain.resize(BloomMipNumber);
    
    Vector2 mimSizeF = { static_cast<float_t>(viewportSize.x) , static_cast<float_t>(viewportSize.y) };
    Vector2i mimSize = { static_cast<int32_t>(viewportSize.x) , static_cast<int32_t>(viewportSize.y) };

    for (size_t i = 0; i < mipChain.size(); i++)
    {
        mimSizeF *= 0.5f;
        mimSize.x /= 2;
        mimSize.y /= 2;
        mipChain[i].sizef = mimSizeF;
        
        const TextureCreateInfo createInfo =
        {
            .size = { static_cast<int32_t>(mimSizeF.x), static_cast<int32_t>(mimSizeF.y) },
            .filtering = TextureFiltering::Linear,
            .wrapping = TextureWrapping::ClampToEdge,
            .format = TextureFormat::Rgba,
            .internalFormat = BloomTextureFormat,
            .dataType = DataType::Float
        };
        
        mipChain[i].texture = new Texture(createInfo);
    }
}
