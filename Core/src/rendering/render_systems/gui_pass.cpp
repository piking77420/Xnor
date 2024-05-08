#include "rendering/render_systems/gui_pass.hpp"

#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"
#include "scene/component/button.hpp"

using namespace XnorCore;

void GuiPass::RenderGui(const Scene& scene, const Vector2i viewPortSize) const
{
    scene.GetAllComponentOfType<Image>(&m_Images);
    scene.GetAllComponentOfType<Button>(&m_Buttons);

    m_GuiShader->Use();
    const Matrix matrixProj = Matrix::Orthographic(0.f, static_cast<float_t>(viewPortSize.x), 0.f,static_cast<float_t>(viewPortSize.y),0.1f, 1000.f);
    constexpr Matrix matrixView = Matrix::Identity();
    m_GuiShader->SetMat4("projection",matrixProj);
    m_GuiShader->SetMat4("view",matrixView);
    RenderImage(viewPortSize);

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

void GuiPass::RenderImage(const Vector2i viewPortSize) const
{
    for(const Image* image : m_Images)
    {
        if (!image->image.IsValid())
            continue;

        image->image->BindTexture(0);
        Vector3 pos = static_cast<Vector3>(image->screenTransform);
        pos.z = -1.f;
        pos.y = static_cast<float_t>(viewPortSize.y) - pos.y; // Flip y axis to mach mouse position
        m_GuiShader->SetMat4("model",Matrix::Trs(pos,Quaternion::Identity(),static_cast<Vector3>(image->size)));

        Rhi::DrawModel(DrawMode::Triangles, m_Quad->GetId());
        
    }
}
