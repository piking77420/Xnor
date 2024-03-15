#include "editing/picking_strategy.hpp"
#include "editor.hpp"
#include "rendering/rhi_typedef.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorEditor;

PickingStrategy::PickingStrategy(Editor* editor) : m_Editor(editor)
{
    const Vector2i windowSize = XnorCore::Window::GetSize();
    InitRendering(windowSize);
    
    m_PickingShader = XnorCore::ResourceManager::Get<XnorCore::Shader>("picking_shader");
    m_PickingShader->CreateInRhi();
    
}

PickingStrategy::~PickingStrategy()
{
    DestroyRendering();
}

void PickingStrategy::ResizeHandle(Vector2i newSize)
{
    if(frameBuffer->GetSize() != newSize)
    {
        DestroyRendering();
        InitRendering(newSize);
    }
}

bool PickingStrategy::GetEntityFromScreen(
    Vector2i pixelPos,
    XnorCore::Scene& scene,
    const XnorCore::Camera& pointOfView,
    XnorCore::Entity** entity
)
{
    if (!m_Editor)
        return false;

    if (!frameBuffer)
        return false;

    // Draw
    m_PickingShader->Use();
    XnorCore::RenderPassBeginInfo beginInfo =
    {
        .frameBuffer = frameBuffer,
        .renderAreaOffset = { 0, 0 },
        .renderAreaExtent = frameBuffer->GetSize(),
        .clearBufferFlags = static_cast<XnorCore::BufferFlag::BufferFlag>(XnorCore::BufferFlag::ColorBit | XnorCore::BufferFlag::DepthBit),
        .clearColor = Vector4(0.f)
    };
    m_Editor->renderer.RenderNonShaded(pointOfView,beginInfo,m_ColorPass,m_PickingShader,scene,true);
    m_PickingShader->Unuse();

    
    float_t getValue = 0.f;
    frameBuffer->GetPixelFromAttachment(0,pixelPos, XnorCore::TextureFormat::Red, XnorCore::DataType::Float,&getValue);

    if(getValue == 0.f)
        return false;

    // - 1 cause in render wee need to do +1 to avoid the black color of the attachement be a valid index  
    const uint32_t EntityIndex = static_cast<uint32_t>(getValue) - 1;

    const XnorCore::List<XnorCore::Entity*>& entities = scene.GetEntities();
    
    for (size_t i = 0 ; i < entities.GetSize(); i++)
    {
        if(EntityIndex == i)
        {
            *entity = entities[i];
            return true;
        }
    }
    
    return false;
}



void PickingStrategy::DestroyRendering() const
{
    delete frameBuffer;
    delete m_ColorAttachment;
    delete m_DepthAttachement;
}

void PickingStrategy::InitRendering(const Vector2i size)
{
    frameBuffer = new XnorCore::FrameBuffer(size);
    m_ColorAttachment = new XnorCore::Texture(XnorCore::TextureInternalFormat::R32F,size);
    m_DepthAttachement = new XnorCore::Texture(XnorCore::TextureInternalFormat::DepthComponent16,size);

    const std::vector<XnorCore::RenderTargetInfo> attachementsType =
    {
        {
            XnorCore::Attachment::Color00
        },
        {
            XnorCore::Attachment::Depth
        }
    };

    m_ColorPass = XnorCore::RenderPass(attachementsType);
    const std::vector<const XnorCore::Texture*> targets = { m_ColorAttachment, m_DepthAttachement };
    frameBuffer->Create(m_ColorPass,targets);

}
