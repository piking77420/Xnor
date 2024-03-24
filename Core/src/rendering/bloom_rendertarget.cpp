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
}

void BloomRenderTarget::Destroy()
{
    for (const BloomMip& mip : m_MipChain)
        delete mip.texture;

    delete m_ThresholdTexture;
}

void BloomRenderTarget::Resize(const Vector2i viewportSize)
{
    Destroy();
    Initialize(viewportSize);
}

void BloomRenderTarget::CreateBloomMip(const Vector2i viewportSize)
{
     
    Vector2 mimSizeF = { static_cast<float_t>(viewportSize.x) , static_cast<float_t>(viewportSize.y) };
    Vector2i mimSize = { static_cast<int32_t>(viewportSize.x) , static_cast<int32_t>(viewportSize.y) };

    for (size_t i = 0; i < m_MipChain.size(); i++)
    {
        mimSizeF *= 0.5f;
        mimSize.x /= 2;
        mimSize.y /= 2;
        m_MipChain[i].sizef = mimSizeF;
        
        const TextureCreateInfo createInfo =
        {
            .data = nullptr,
            .size = { static_cast<int32_t>(mimSizeF.x), static_cast<int32_t>(mimSizeF.y) },
            .filtering = TextureFiltering::Linear,
            .wrapping = TextureWrapping::ClampToEdge,
            .format = TextureFormat::Rgba,
            .internalFormat = BloomTextureFormat,
            .dataType = DataType::Float
        };
        
        m_MipChain[i].texture = new Texture(createInfo);
    }
}
