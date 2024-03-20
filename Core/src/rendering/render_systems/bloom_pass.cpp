#include "rendering/render_systems/bloom_pass.hpp"

#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorCore;

void BloomPass::Init(uint32_t bloomMips)
{
    m_BloomMips = bloomMips;
    m_FrameBuffer = new FrameBuffer(Window::GetSize());
    m_ThresholdTexture = new Texture(TextureInternalFormat::R11FG11FB10F,m_FrameBuffer->GetSize());
    m_MipChain.resize(m_BloomMips);

    
    if (m_Quad != nullptr && m_DownSample != nullptr && m_UpSample != nullptr)
       return; 
    
    m_Quad = ResourceManager::Get<Model>("assets/models/quad.obj");
    // Downsample
    m_DownSample = ResourceManager::Get<Shader>("down_sample");
    m_DownSample->CreateInRhi();
    m_DownSample->Use();
    m_DownSample->SetInt("srcTexture", 0);
    m_DownSample->Unuse();

    // Upsample
    m_UpSample = ResourceManager::Get<Shader>("up_sample");
    //m_UpSample->SetBlendFunction( {true, BlendValue::One, BlendValue::One, BlendEquation::Add });
    m_UpSample->CreateInRhi();
    m_UpSample->Use();
    m_UpSample->SetInt("currentMip", 0);
    m_UpSample->SetInt("nextMip", 1);    
    m_UpSample->Unuse();

    m_TresholdFilter = ResourceManager::Get<Shader>("bloom_threshold");
    m_TresholdFilter->CreateInRhi();
    m_TresholdFilter->Use();
    m_TresholdFilter->SetInt("srcTexture",0);
    m_TresholdFilter->Unuse();


}

void BloomPass::ComputeBloom(const Texture& textureWithoutBloom)
{
    HandleBlooMip(textureWithoutBloom.GetSize());
    ThresholdFilter(textureWithoutBloom);
    DownSampling();
    UpSampling();
}

Texture* BloomPass::GetBloomTexture() const
{
    return m_MipChain[0].texture;
}

void BloomPass::UpSampling()
{
    m_UpSample->Use();
    m_UpSample->SetFloat("filterRadius", m_FilterRadius);

    for (size_t i = m_MipChain.size() - 1; i > 0; i--)
    {
        const BloomMip& mip = m_MipChain[i];
        const BloomMip& nextMip = m_MipChain[i - 1];
        m_UpSample->BindTexture(0, *mip.texture, 0, false, 0, ImageAccess::ReadWrite);
        m_UpSample->BindTexture(1, *nextMip.texture, 0, false, 0, ImageAccess::ReadWrite);

        m_UpSample->DispatchCompute(std::ceil(mip.texture->GetSize().x), std::ceil(mip.texture->GetSize().y) ,1);
        m_UpSample->SetMemoryBarrier(GpuMemoryBarrier::AllBarrierBits);
    }

    
    m_UpSample->Unuse();

}

void BloomPass::DownSampling()
{
    m_DownSample->Use();
    
    m_ThresholdTexture->BindTexture(0);
    const Vector2 textureSizef = { static_cast<float_t>(m_ThresholdTexture->GetSize().x), static_cast<float_t>(m_ThresholdTexture->GetSize().y) } ;
    m_DownSample->SetVec2("srcResolution",textureSizef);
    for (BloomMip& bloomMip : m_MipChain)
    {
        m_FrameBuffer->AttachTexture(*bloomMip.texture, Attachment::Color00, 0);
        const RenderPassBeginInfo renderPassBeginInfo =
        {
            .frameBuffer = m_FrameBuffer,
            .renderAreaOffset = { 0,0 },
            .renderAreaExtent = bloomMip.texture->GetSize(),
            .clearBufferFlags = BufferFlag::None,
        };
        
        m_RenderPass.BeginRenderPass(renderPassBeginInfo);
        Rhi::DrawModel(m_Quad->GetId());
        m_RenderPass.EndRenderPass();
        
        m_DownSample->SetVec2("srcResolution", bloomMip.sizef);
        bloomMip.texture->BindTexture(0);
     
    }
    
    m_ThresholdTexture->UnbindTexture(0);
    m_DownSample->Unuse();
}

void BloomPass::HandleBlooMip(const Vector2i currentViewPortSize)
{
    if (m_FrameBuffer->GetSize() == currentViewPortSize)
        return;
    
    delete m_FrameBuffer;
    delete m_ThresholdTexture;
    
    m_ThresholdTexture = nullptr;
    m_FrameBuffer = nullptr;
    
    for (const BloomMip& mip : m_MipChain)
    {
        delete mip.texture;
    }
    m_FrameBuffer = new FrameBuffer(currentViewPortSize);
    m_ThresholdTexture = new Texture(TextureInternalFormat::R11FG11FB10F,m_FrameBuffer->GetSize());
    
    
    const Vector2i currentBufferSize = m_FrameBuffer->GetSize();
    Vector2 mimSizeF = { static_cast<float_t>(currentBufferSize.x) , static_cast<float_t>(currentBufferSize.y) };
    Vector2i mimSize = { static_cast<int32_t>(currentBufferSize.x) , static_cast<int32_t>(currentBufferSize.y) };

    for (size_t i = 0; i < m_MipChain.size(); i++)
    {
        mimSizeF *= 0.5f;
        mimSize.x /= 2;
        mimSize.y /= 2;
        m_MipChain[i].sizef = mimSizeF;
        
        const TextureCreateInfo createInfo =
        {
            .data = nullptr,
            .size = { static_cast<int32_t>(mimSizeF.x), static_cast<int32_t>(mimSizeF.y) },
            .filtering = TextureFiltering::Linear,
            .wrapping = TextureWrapping::ClampToEdge,
            .format = TextureFormat::Rgb,
            .internalFormat = TextureInternalFormat::R11FG11FB10F,
            .dataType = DataType::Float
        };
        
        m_MipChain[i].texture = new Texture(createInfo);
    }
    
    m_FrameBuffer->AttachTexture(*m_MipChain[0].texture,Attachment::Color00,0);

}

void BloomPass::ThresholdFilter(const Texture& textureToCompute)
{
    m_FrameBuffer->AttachTexture(*m_ThresholdTexture, Attachment::Color00, 0);
    m_TresholdFilter->Use();
    
    textureToCompute.BindTexture(0);
    const RenderPassBeginInfo renderPassBeginInfo =
    {
        .frameBuffer = m_FrameBuffer,
        .renderAreaOffset = { 0,0 },
        .renderAreaExtent = m_ThresholdTexture->GetSize(),
        .clearBufferFlags = BufferFlag::None,
    };

    m_RenderPass.BeginRenderPass(renderPassBeginInfo);
    Rhi::DrawModel(m_Quad->GetId());
    m_TresholdFilter->Unuse();
    textureToCompute.UnbindTexture(0);
    m_RenderPass.EndRenderPass();
    
}
