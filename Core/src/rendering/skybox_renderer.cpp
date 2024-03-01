#include "..\..\include\rendering\skybox_renderer.hpp"

#include <glad/glad.h>

#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorCore;

SkyboxRenderer::SkyboxRenderer() : m_Cubemap(nullptr)
{
    
}

SkyboxRenderer::~SkyboxRenderer()
{
    delete m_Cubemap;   
}

void SkyboxRenderer::InitializeResources()
{
    m_SkyboxDrawerShader = ResourceManager::Get<Shader>("skymap_shader");
    m_SkyboxDrawerShader->CreateInRhi();
    m_SkyboxDrawerShader->Use();
    m_SkyboxDrawerShader->SetInt("skybox",0);
    
    std::array<std::string,6> testCubeMap
    {
        "assets/skybox/right.jpg",
        "assets/skybox/left.jpg",
        "assets/skybox/top.jpg",
        "assets/skybox/bottom.jpg",
        "assets/skybox/front.jpg",
        "assets/skybox/back.jpg"
    };
    
    m_Cubemap = new Cubemap(testCubeMap);
    m_Cubemap->CreateInRhi();
}

void SkyboxRenderer::DrawSkymap(const Pointer<Model>& cubeModel) const
{
    // TO DO MOVE THIS 
    glDepthFunc(GL_LEQUAL);
    m_SkyboxDrawerShader->Use();
    m_Cubemap->BindTexture(0);
    Rhi::DrawModel(cubeModel->GetId());
    m_SkyboxDrawerShader->Unuse();
    // TO DO MOVE THIS
    glDepthFunc(GL_LESS);

}
