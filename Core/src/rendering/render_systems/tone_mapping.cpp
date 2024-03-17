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
}



void ToneMapping::ComputeToneMaping(const Texture& imageWithoutToneMapping, const Pointer<Model>& quadModel) const
{
    m_Aces->Use();
    imageWithoutToneMapping.BindTexture(10);
    Rhi::DrawModel(quadModel->GetId());
    m_Aces->Unuse();
}

void ToneMapping::RecompileShader()
{
    m_Aces->Recompile();
}
