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
    frameBuffer = new FrameBuffer(viewPortSize);
    m_BaseImage = new Texture(TextureInternalFormat::Rgb16, viewPortSize);
    m_PostProcessImage = new Texture(TextureInternalFormat::Rgb16, viewPortSize);
    // Set Up renderPass
    const RenderPass renderPass(attachementsType);
    const std::vector<const Texture*> targets = { m_BaseImage };
    frameBuffer->AttachTextures(renderPass,targets);
    
    viewportData.Init(viewPortSize);
    m_PostProcessPass.Init();
}

void Viewport::Destroy()
{
    delete frameBuffer;
    delete m_BaseImage;
    delete m_PostProcessImage;
    viewportData.Destroy();
}

void Viewport::OnResize(Vector2i newSize)
{
    Destroy();
    Init(newSize);
}

void Viewport::ComputePostProcess()
{
    m_PostProcessPass.Compute(*viewportData.colorAttachment,*m_BaseImage);
}

Texture* Viewport::GetImage() const
{
    return m_BaseImage;
}

bool_t Viewport::IsValid() const
{
    return camera != nullptr &&  frameBuffer != nullptr;  
}
