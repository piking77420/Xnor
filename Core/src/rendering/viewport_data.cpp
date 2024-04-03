#include "rendering/viewport_data.hpp"

using namespace XnorCore;

void ViewportData::Init(const Vector2i size)
{
    depthAtttachment = new Texture(TextureInternalFormat::DepthComponent32FStencil8, size);
    InitForward(size);
    InitDeffered(size);
    postprocessRendertarget.Init(size);
}

void ViewportData::Destroy()
{
    delete gFramebuffer;
    delete renderBuffer;
    
    delete positionAttachment;
    delete normalAttachement;
    delete albedoAttachment;
    delete metallicRoughnessReflectance;
    delete ambiantOcclusion;
    delete emissive;
    
    delete colorAttachment;
    delete depthAtttachment;

    postprocessRendertarget.Destroy();
}

void ViewportData::BindDescriptor() const
{
    positionAttachment->BindTexture(DefferedDescriptor::Position);
    normalAttachement->BindTexture(DefferedDescriptor::Normal);
    albedoAttachment->BindTexture(DefferedDescriptor::Albedo);
    metallicRoughnessReflectance->BindTexture(DefferedDescriptor::MetallicRoughessReflectance);
    ambiantOcclusion->BindTexture(DefferedDescriptor::AmbiantOcclusion);
    emissive->BindTexture(DefferedDescriptor::Emissivive);
}

void ViewportData::UnBindDescriptor() const
{
    positionAttachment->UnBindTexture(DefferedDescriptor::Position);
    normalAttachement->UnBindTexture(DefferedDescriptor::Normal);
    albedoAttachment->UnBindTexture(DefferedDescriptor::Albedo);
    metallicRoughnessReflectance->UnBindTexture(DefferedDescriptor::MetallicRoughessReflectance);
    ambiantOcclusion->UnBindTexture(DefferedDescriptor::AmbiantOcclusion);
    emissive->UnBindTexture(DefferedDescriptor::Emissivive);
}

void ViewportData::InitForward(const Vector2i size)
{
    renderBuffer = new Framebuffer();
    colorAttachment = new Texture(TextureInternalFormat::Rgba32F, size, TextureFormat::Rgba);

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

void ViewportData::InitDeffered(const Vector2i size)
{
    gFramebuffer = new Framebuffer();
    positionAttachment = new Texture(TextureInternalFormat::Rgba16F, size);
    normalAttachement = new Texture(TextureInternalFormat::Rgb16F, size);
    albedoAttachment = new Texture(TextureInternalFormat::Rgb16F, size);
    metallicRoughnessReflectance = new Texture(TextureInternalFormat::Rgb16F, size);
    ambiantOcclusion = new Texture(TextureInternalFormat::Rg16F, size);
    
    emissive = new Texture(TextureInternalFormat::Rgba16F, size);
    
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
            .attachment = Attachment::Color05
        },
        {
            .attachment = Attachment::Depth
        },
    };
	
    // Set Up renderPass
    gBufferPass = RenderPass(attachementsType);

    const std::vector<const Texture*> targets = { positionAttachment, normalAttachement,
        albedoAttachment, metallicRoughnessReflectance, ambiantOcclusion, emissive, depthAtttachment};
    gFramebuffer->AttachTextures(gBufferPass, targets);
}
