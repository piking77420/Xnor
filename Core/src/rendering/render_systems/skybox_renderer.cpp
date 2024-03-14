#include "rendering/render_systems/skybox_renderer.hpp"

#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorCore;

void SkyboxRenderer::InitializeResources()
{
    m_SkyboxDrawerShader = ResourceManager::Get<Shader>("skymap_shader");
    m_SkyboxDrawerShader->SetDepthFunction(DepthFunction::LessEqual);
    m_SkyboxDrawerShader->CreateInRhi();
    
    m_SkyboxDrawerShader->Use();
    m_SkyboxDrawerShader->SetInt("skybox",0);
}

void SkyboxRenderer::DrawSkymap(const Pointer<Model>& cubeModel, const Skybox& skybox) const 
{
    m_SkyboxDrawerShader->Use();
    skybox.GetCubeMap()->BindTexture(0);
    Rhi::DrawModel(cubeModel->GetId());
    m_SkyboxDrawerShader->Unuse();
}
