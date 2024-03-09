#include "rendering\viewport.hpp"

#include "world/world.hpp"

using namespace XnorCore;

bool Viewport::GetEntityFromScreen(Vector2i pixelPos, XnorCore::Entity** entity) const
{
    std::vector<const MeshRenderer*> meshRenderers;
    World::scene.GetAllComponentOfType<MeshRenderer>(&meshRenderers);
    float_t getValue;
    viewportData.gframeBuffer->GetPixelFromAttachment(3,pixelPos,TextureFormat::Red,DataType::Float,&getValue);
    
    if(getValue == 0)
    {
        return false;
    }
    
    uint32_t value = static_cast<uint32_t>(getValue) - 1;
    
    
    for (size_t i = 0; i < meshRenderers.size(); ++i)
    {
        if (i == value)
        {
            *entity = meshRenderers[i]->entity;
            return true;
        }
    }
    
    return false;
}

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
    frameBuffer->Create(renderPass,targets);
    
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
