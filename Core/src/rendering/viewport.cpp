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
    finalImage = new Texture(TextureInternalFormat::Rgb16, viewPortSize);
    // Set Up renderPass
    const RenderPass renderPass(attachementsType);
    const std::vector<const Texture*> targets = { finalImage };
    frameBuffer->AttachTextures(renderPass,targets);
    
    viewportData.Init(viewPortSize);
}

void Viewport::Destroy()
{
    delete frameBuffer;
    delete finalImage;
    
    viewportData.Destroy();
}

void Viewport::OnResize(Vector2i newSize)
{
    Destroy();
    Init(newSize);
}

bool_t Viewport::IsValid() const
{
    return camera != nullptr &&  frameBuffer != nullptr;  
}
