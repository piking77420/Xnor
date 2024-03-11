#include "rendering/viewport_data.hpp"

using namespace XnorCore;

void ViewportData::Init(Vector2i windowSize)
{
    depthAtttachment = new Texture(TextureInternalFormat::DepthComponent32FStencil8, windowSize);
    InitForward(windowSize);
    InitDeffered(windowSize);
}

void ViewportData::Destroy()
{
    delete gframeBuffer;
    delete renderBuffer;
    
    delete positionAtttachment;
    delete normalAttachement;
    delete albedoAttachment;

    delete colorAttachment;
    delete depthAtttachment;
}

void ViewportData::InitForward(Vector2i windowSize)
{
    renderBuffer = new FrameBuffer(windowSize);
    colorAttachment = new Texture(TextureInternalFormat::Rgb16F, windowSize);

    const std::vector<RenderTargetInfo> attachementsType =
    {
        {
            .attachment = Attachment::Color00,
        },
        {
            .attachment = Attachment::Depth,
        }
       
    };
	
    // Set Up renderPass
    colorPass = RenderPass(attachementsType);
    const std::vector<const Texture*> targets = { colorAttachment, depthAtttachment };
    renderBuffer->Create(colorPass, targets);
}

void ViewportData::InitDeffered(Vector2i windowSize)
{
    gframeBuffer = new FrameBuffer(windowSize);
    positionAtttachment = new Texture(TextureInternalFormat::Rgb16F, windowSize);
    normalAttachement = new Texture(TextureInternalFormat::Rgb16F, windowSize);
    albedoAttachment = new Texture(TextureInternalFormat::Rgb16F, windowSize);
	
    const std::vector<RenderTargetInfo> attachementsType =
    {
        {
            .attachment = Attachment::Color00
        },
        {
            .attachment = Attachment::Color01
        },
        {
            .attachment = Attachment::Color02
            
        },
        {
            .attachment = Attachment::Depth
        },
    };
	
    // Set Up renderPass
    gbufferPass = RenderPass(attachementsType);

    const std::vector<const Texture*> targets = { positionAtttachment, normalAttachement, albedoAttachment, depthAtttachment};
    gframeBuffer->Create(gbufferPass, targets);
}
