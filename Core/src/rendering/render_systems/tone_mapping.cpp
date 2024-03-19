#include "rendering/render_systems/tone_mapping.hpp"

#include "rendering/frame_buffer.hpp"
#include "rendering/render_pass.hpp"
#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorCore;

void ToneMapping::ComputeToneMaping(const Texture& imageWithoutToneMapping, const Texture& bloomTexture) const
{
    Rhi::SetViewport({ 0, 0 }, imageWithoutToneMapping.GetSize());

    m_TestCompute->Use();
    imageWithoutToneMapping.BindTexture(0);
    Rhi::BindImageTexture(0,imageWithoutToneMapping.GetId(), 0,false,0,ImageAccess::WriteOnly,TextureInternalFormat::Rgba16F);
    Rhi::DispactCompute(imageWithoutToneMapping.GetSize().x / 10,imageWithoutToneMapping.GetSize().y / 10,1);
    Rhi::GpuMemoryBarrier(MemoryBarrier::AllBarrierBits);
    m_TestCompute->Unuse();
    

    m_Aces->Use();
    imageWithoutToneMapping.BindTexture(ToneMapedTextureBinding);
    bloomTexture.BindTexture(1);
    Rhi::DrawModel(m_Quad->GetId());
    m_Aces->Unuse();

  
}

void ToneMapping::InitializeResources()
{
    if (m_Aces == nullptr || m_Quad == nullptr)
    {
        m_Aces = ResourceManager::Get<Shader>("tone_mapper");
        m_Aces->CreateInRhi();
        m_Aces->Use();
        m_Aces->SetInt("beforeToneMappedImage", ToneMapedTextureBinding);
        m_Aces->SetInt("bloomBlur",1);
        m_Aces->Unuse();

        m_Quad = ResourceManager::Get<Model>("assets/models/quad.obj");

        m_TestCompute = ResourceManager::Get<Shader>("test_compute");
        m_TestCompute->CreateInRhi();
        m_TestCompute->Use();
        m_TestCompute->SetInt("screen",0);
        m_TestCompute->Unuse();
    }
}

void ToneMapping::RecompileShader()
{
    m_Aces->Recompile();
}
