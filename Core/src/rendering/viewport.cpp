#include "rendering\viewport.hpp"

#include "world/world.hpp"

using namespace XnorCore;


void Viewport::Init(Vector2i initViewportSize)
{
    viewPortSize = initViewportSize;

    using namespace XnorCore;
    const std::vector<RenderTargetInfo> attachementsType =
    {
        {
            Attachment::Color00
        }
    };
    // Init Rendering
    frameBuffer = new FrameBuffer();
    m_Image = new Texture(TextureInternalFormat::Rgba32F, viewPortSize, TextureFormat::Rgb);
    // Set Up renderPass
    const RenderPass renderPass(attachementsType);
    const std::vector<const Texture*> targets = { m_Image };
    frameBuffer->AttachTextures(renderPass,targets);
    
    viewportData.Init(viewPortSize);
}

void Viewport::Destroy()
{
    delete frameBuffer;
    delete m_Image;
    viewportData.Destroy();
}

void Viewport::OnResize(const Vector2i newSize)
{
    Destroy();
    Init(newSize);
}

Texture* Viewport::GetImage() const
{
    return m_Image;
}

bool_t Viewport::IsValid() const
{
    return camera != nullptr &&  frameBuffer != nullptr;  
}
