#include "rendering/render_systems/gui_pass.hpp"

#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"
#include "scene/component/button.hpp"

using namespace XnorCore;

void GuiPass::RenderGui(const Scene& scene, const Viewport& viewport) const
{
    scene.GetAllComponentOfType<Image>(&m_Images);
    scene.GetAllComponentOfType<Button>(&m_Buttons);

    m_GuiShader->Use();
    const Matrix matrixProj = Camera::Ortho(0.f, viewport.viewPortSize.x, 0.f, viewport.viewPortSize.y,0.1f, 1000.f);
    const Matrix matrixView = Matrix::Identity();
    m_GuiShader->SetMat4("projection",matrixProj);
    m_GuiShader->SetMat4("view",matrixView);
    RenderImage();

    m_GuiShader->Unuse();
}

void GuiPass::Init()
{
    m_GuiShader = ResourceManager::Get<Shader>("gui_shader");
    constexpr BlendFunction blendFunction =
    {
        .isBlending = true,
        .sValue = BlendValue::SrcAlpha,
        .dValue = BlendValue::OneMinusSrcAlpha
    };
	
    m_GuiShader->SetBlendFunction(blendFunction);
    m_GuiShader->CreateInInterface();
    m_GuiShader->Use();
    m_GuiShader->SetInt("uiTexture",0);
    m_GuiShader->Unuse();

    
    m_Quad = ResourceManager::Get<Model>("assets/models/quad.obj");
    
}

void GuiPass::RenderButton()
{
    
}

void GuiPass::RenderImage() const
{
    for(const Image* image : m_Images)
    {
        if (!image->image.IsValid())
            continue;

        image->image->BindTexture(0);
        m_GuiShader->SetMat4("model",Matrix::Trs(static_cast<Vector3>(image->screenTransform),Quaternion::Identity(),static_cast<Vector3>(image->size)));

        Rhi::DrawModel(DrawMode::Triangles, m_Quad->GetId());
        
    }
}
