#include "rendering/viewport_data.hpp"

using namespace XnorCore;

void ViewportData::Init(Vector2i windowSize)
{
    depthAtttachment = new Texture(TextureInternalFormat::DepthComponent32FStencil8, windowSize);
    InitForward(windowSize);
    InitDeffered(windowSize);
    postprocessRendertarget.Init(windowSize);
}

void ViewportData::Destroy()
{
    delete gframeBuffer;
    delete renderBuffer;
    
    delete positionAtttachment;
    delete normalAttachement;
    delete albedoAttachment;
    delete metallicRougnessReflectance;
    delete emissiveAmbiantOcclusion;

    delete colorAttachment;
    delete depthAtttachment;
    postprocessRendertarget.Destroy();
}

void ViewportData::InitForward(Vector2i windowSize)
{
    renderBuffer = new FrameBuffer();
    colorAttachment = new Texture(TextureInternalFormat::Rgba32F, windowSize, TextureFormat::Rgba);

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
    renderBuffer->AttachTextures(colorPass, targets);
}

void ViewportData::InitDeffered(Vector2i windowSize)
{
    gframeBuffer = new FrameBuffer();
    positionAtttachment = new Texture(TextureInternalFormat::Rgb16F, windowSize);
    normalAttachement = new Texture(TextureInternalFormat::Rgb16F, windowSize);
    albedoAttachment = new Texture(TextureInternalFormat::Rgb16F, windowSize);
    metallicRougnessReflectance = new Texture(TextureInternalFormat::Rgb16F,windowSize);
    emissiveAmbiantOcclusion = new Texture(TextureInternalFormat::Rg16F,windowSize);
    
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
            .attachment = Attachment::Color03
                    
        },
        {
            .attachment = Attachment::Color04
                    
        },
        {
            .attachment = Attachment::Depth
        },
    };
	
    // Set Up renderPass
    gbufferPass = RenderPass(attachementsType);

    const std::vector<const Texture*> targets = { positionAtttachment, normalAttachement,
        albedoAttachment, metallicRougnessReflectance, emissiveAmbiantOcclusion, depthAtttachment};
    gframeBuffer->AttachTextures(gbufferPass, targets);
}
