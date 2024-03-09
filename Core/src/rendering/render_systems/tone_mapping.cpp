#include "rendering/render_systems/tone_mapping.hpp"

#include "rendering/frame_buffer.hpp"
#include "rendering/render_pass.hpp"
#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorCore;

void ToneMapping::InitializeResources()
{
    m_Aces = ResourceManager::Get<Shader>("tone_mapper");
    m_Aces->CreateInRhi();
    m_Aces->Use();
    m_Aces->SetInt("beforeToneMappedImage", 10);
    m_Aces->Unuse();
    
    const std::vector<RenderTargetInfo> attachementsType =
   {
        {
            .attachment = Attachment::Color00,.isDrawingOn = false
        },
    };
    // Set Up renderPass
    m_ToneMappingRenderPass = RenderPass(attachementsType);
}

void ToneMapping::OnResize(const Vector2i windowSize)
{
    m_FrameBuffer = new FrameBuffer(windowSize);
    m_ColorAttachment = new Texture(TextureInternalFormat::Rgb16F, windowSize);
   
    const std::vector<const Texture*> targets = { m_ColorAttachment };

    m_FrameBuffer->Create(m_ToneMappingRenderPass, targets);
}

void ToneMapping::ComputeToneMaping(const Texture& imageWithoutToneMapping, const Pointer<Model>& quadModel) const
{
    
    RenderPassBeginInfo renderPassBeginInfo =
    {
        .frameBuffer = m_FrameBuffer,
        .renderAreaOffset = { 0, 0},
        .renderAreaExtent = m_FrameBuffer->GetSize(),
        .clearBufferFlags = static_cast<BufferFlag>(BufferFlagColorBit | BufferFlagDepthBit),
        .clearColor = { 0, 0, 0, 0 }
    };
    
    m_ToneMappingRenderPass.BeginRenderPass(renderPassBeginInfo);

    m_Aces->Use();
    imageWithoutToneMapping.BindTexture(10);
    Rhi::DrawQuad(quadModel->GetId());
    m_Aces->Unuse();
    m_ToneMappingRenderPass.EndRenderPass();  
}

const Texture& ToneMapping::GetToneMapedImage() const
{
    return *m_ColorAttachment;
}

void ToneMapping::OnResizeWindow(const Vector2i windowSize)
{
    Destroy();
    OnResize(windowSize);
}

void ToneMapping::Destroy() const
{
    delete m_FrameBuffer;
    delete m_ColorAttachment;
}

void ToneMapping::RecompileShader()
{
    m_Aces->Recompile();
}
