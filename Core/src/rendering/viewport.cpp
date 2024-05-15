#include "rendering\viewport.hpp"

#include "world/world.hpp"

using namespace XnorCore;

void Viewport::Init(const Vector2i size)
{
    viewPortSize = size;
 
    using namespace XnorCore;
    const std::vector<RenderTargetInfo> attachementsType =
    {
        {
            Attachment::Color00
        }
    };

    // Init Rendering
    frameBuffer = new Framebuffer();
    m_Image = new Texture(TextureInternalFormat::Rgba32F, viewPortSize, TextureFormat::Rgb);

    // Set Up renderPass
    const RenderPass renderPass(attachementsType);
    const std::vector<const Texture*> targets = { m_Image };
    frameBuffer->AttachTextures(renderPass, targets);
    
    viewportData.Init(viewPortSize);
}

void Viewport::Destroy()
{
    delete frameBuffer;
    delete m_Image;

    viewportData.Destroy();
}

void Viewport::Resize(const Vector2i newSize)
{
    Destroy();
    Init(newSize);
}

float_t Viewport::GetAspect() const
{
    return static_cast<float_t>(viewPortSize.x) / static_cast<float_t>(viewPortSize.y); 
}
