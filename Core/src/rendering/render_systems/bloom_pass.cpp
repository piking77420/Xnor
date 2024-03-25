#include "rendering/render_systems/bloom_pass.hpp"

#include "input/keyboard_input.hpp"
#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorCore;

void BloomPass::Init()
{
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
    m_UpSample = ResourceManager::Get<ComputeShader>("up_sample");
    m_UpSample->CreateInRhi();
    m_UpSample->Use();
    m_UpSample->SetInt("currentMip", 0);
    m_UpSample->SetInt("nextMip", 1);    
    m_UpSample->Unuse();

    m_ThresholdFilter = ResourceManager::Get<Shader>("bloom_threshold");
    m_ThresholdFilter->CreateInRhi();
    m_ThresholdFilter->Use();
    m_ThresholdFilter->SetInt("srcTexture",0);
    m_ThresholdFilter->Unuse();
    
}


void BloomPass::ComputeBloom(const Texture& imageWithoutBloom, const BloomRenderTarget& bloomRenderTarget) const
{
    ThresholdFilter(imageWithoutBloom, bloomRenderTarget);
    DownSampling(bloomRenderTarget);
    UpSampling(bloomRenderTarget);
}

void BloomPass::UpSampling(const BloomRenderTarget& bloomRenderTarget) const
{   
    const std::vector<BloomRenderTarget::BloomMip>& mipchain = bloomRenderTarget.mipChain;
    
    m_UpSample->Use();
    m_UpSample->SetFloat("bloom_intensity",  1.f);

    for (size_t i = mipchain.size() - 1; i > 0; i--)
    {
        const BloomRenderTarget::BloomMip& mip = mipchain[i];
        const BloomRenderTarget::BloomMip& nextMip = mipchain[i - 1];

        const Vector2 mipSize =  { std::floor(nextMip.sizef.x),std::floor(nextMip.sizef.y) };
        m_UpSample->SetVec2("u_texel_size",Vector2(1.0f) / mipSize);
        
        // Source
        mip.texture->BindTexture(0);
        // Target
        m_UpSample->BindTexture(1, *nextMip.texture, 0, false, 0, ImageAccess::ReadWrite);

        m_UpSample->DispatchCompute(static_cast<uint32_t>(std::ceil(mipSize.x/ 8.f)), static_cast<uint32_t>(std::ceil(mipSize.y/ 8.f)) ,1);  

        m_UpSample->SetMemoryBarrier(GpuMemoryBarrier::AllBarrierBits);
    }
    
    m_UpSample->Unuse();

}

void BloomPass::DownSampling(const BloomRenderTarget& bloomRenderTarget) const
{
    m_DownSample->Use();
    
    bloomRenderTarget.thresholdTexture->BindTexture(0);
    const Vector2 textureSizef = { static_cast<float_t>(bloomRenderTarget.thresholdTexture->GetSize().x), static_cast<float_t>(bloomRenderTarget.thresholdTexture->GetSize().y) } ;
    m_DownSample->SetVec2("srcResolution",textureSizef);
    for (const BloomRenderTarget::BloomMip& bloomMip : bloomRenderTarget.mipChain)
    {
        bloomRenderTarget.frameBuffer->AttachTexture(*bloomMip.texture, Attachment::Color00, 0);
        const RenderPassBeginInfo renderPassBeginInfo =
        {
            .frameBuffer =  bloomRenderTarget.frameBuffer,
            .renderAreaOffset = { 0,0 },
            .renderAreaExtent = bloomMip.texture->GetSize(),
            .clearBufferFlags = BufferFlag::None,
        };
        
        bloomRenderTarget.renderPass.BeginRenderPass(renderPassBeginInfo);
        Rhi::DrawModel(m_Quad->GetId());
         bloomRenderTarget.renderPass.EndRenderPass();
        
        m_DownSample->SetVec2("srcResolution", bloomMip.sizef);
        bloomMip.texture->BindTexture(0);
     
    }
    
    bloomRenderTarget.thresholdTexture->UnbindTexture(0);
    m_DownSample->Unuse();
}

void BloomPass::ThresholdFilter(const Texture& imageWithoutBloom, const BloomRenderTarget& bloomRenderTarget) const
{
    const Texture& thresholdTexture = *bloomRenderTarget.thresholdTexture;
    
    bloomRenderTarget.frameBuffer->AttachTexture(thresholdTexture, Attachment::Color00, 0);
    m_ThresholdFilter->Use();
    
    imageWithoutBloom.BindTexture(0);
    const RenderPassBeginInfo renderPassBeginInfo =
    {
        .frameBuffer = bloomRenderTarget.frameBuffer,
        .renderAreaOffset = { 0,0 },
        .renderAreaExtent = thresholdTexture.GetSize(),
        .clearBufferFlags = BufferFlag::None,
    };

    bloomRenderTarget.renderPass.BeginRenderPass(renderPassBeginInfo);
    Rhi::DrawModel(m_Quad->GetId());
    m_ThresholdFilter->Unuse();
    imageWithoutBloom.UnbindTexture(0);
    bloomRenderTarget.renderPass.EndRenderPass();
    
}
