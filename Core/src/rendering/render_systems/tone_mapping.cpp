#include "rendering/render_systems/tone_mapping.hpp"

#include "rendering/frame_buffer.hpp"
#include "rendering/render_pass.hpp"
#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorCore;

void ToneMapping::ComputeToneMaping(const Texture& imageWithoutToneMapping, const Texture& bloomTexture) const
{
    Rhi::SetViewport({ 0, 0 }, imageWithoutToneMapping.GetSize());
    
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
    }
}

void ToneMapping::RecompileShader()
{
    m_Aces->Recompile();
}
