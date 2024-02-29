#include "rendering/tone_mapping.hpp"

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
}

void ToneMapping::Initialize(const Vector2i windowSize)
{
    m_FrameBuffer = new FrameBuffer(windowSize);
    m_ColorAttachment = new Texture(TextureInternalFormat::Rgb16F, windowSize);
    const std::vector<RenderTargetInfo> attachementsType =
    {
        {
            .attachment = Attachment::Color01,
            .draw = true,
            .isClearing = true
        },
    };
    // Set Up renderPass
    const RenderPass renderPass(attachementsType);
    const std::vector<const Texture*> targets = { m_ColorAttachment };

    m_FrameBuffer->Create(renderPass,targets);
}

void ToneMapping::ComputeToneMaping(const Texture& imageWithoutToneMapping, const Pointer<Model>& cubeMode) const
{
    m_FrameBuffer->BindFrameBuffer();

    m_Aces->Use();
    imageWithoutToneMapping.BindTexture(10);
    Rhi::DrawQuad(cubeMode->GetId());
    m_Aces->Unuse();
    m_FrameBuffer->UnBindFrameBuffer();
}

const Texture& ToneMapping::GetToneMapedImage() const
{
    return *m_ColorAttachment;
}

void ToneMapping::OnResizeWindow(const Vector2i windowSize)
{
    Destroy();
    Initialize(windowSize);
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
