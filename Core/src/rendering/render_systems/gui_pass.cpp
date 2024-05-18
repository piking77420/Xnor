#include "rendering/render_systems/gui_pass.hpp"

#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"
#include "scene/component/button.hpp"

using namespace XnorCore;

void GuiPass::RenderGui(const Scene& scene, const Vector2i& viewportSize) const
{
    const Vector2 size = static_cast<Vector2>(viewportSize);
    
    scene.GetAllComponentsOfType<Image>(&m_Images);
    scene.GetAllComponentsOfType<TextComponent>(&m_TextComponents);
    const Matrix matrixProj = Matrix::Orthographic(0.f, size.x, 0.f ,size.y, 0.1f, 1000.f);

    m_FontShader->Use();
    m_FontShader->SetMat4("projection", matrixProj); 
    RenderText();
    m_FontShader->Unuse();
    
    m_GuiShader->Use();
    m_GuiShader->SetMat4("projection", matrixProj);
    RenderImage(size);
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
    m_GuiShader->SetInt("uiTexture", 0);
    m_GuiShader->Unuse();
    
    m_FontShader = ResourceManager::Get<Shader>("font_rendering");

    m_FontShader->SetBlendFunction(blendFunction);
    m_FontShader->CreateInInterface();
    m_FontShader->Use();
    m_FontShader->SetInt("text",0);
    m_FontShader->Unuse();
    
    m_Quad = ResourceManager::Get<Model>("assets/models/quad.obj");
    
    InitSliderQuad();
}

void GuiPass::InitSliderQuad()
{
    m_FontQuadVao.Init();
    m_FontQuadVbo.Init();
    const Vector4 vertices[6] = {
        // Vertex coordinates   // Texture coordinates
        { 0.5f,  0.5f,       1.0f, 1.0f}, 
        { 0.5f, -0.5f,       1.0f, 0.0f},  
        {-0.5f, -0.5f,       0.f,  0.0f},
        
        { 0.5f,  0.5f,       1.0f, 1.0f},
        {-0.5f, -0.5f,       0.0f, 0.0f}, 
        {-0.5f,  0.5f,       0.0f, 1.0f}
    };

    constexpr size_t size = sizeof(vertices);
    m_FontQuadVbo.Allocate(size, &vertices[0].x, BufferUsage::DynamicDraw);
    
    constexpr VertexAttributeBinding vertexAttributeBinding =
    {
        .attribIndex = 0,
        .bindingIndex = 0
    };
    
    constexpr VertexAttribFormat vertexAttribFormat =
    {
        .attribIndex = 0,
        .size = 4,
        .type = DataType::Float,
        .normalized = false,
        .relativeOffset = 0
    };

    const VaoDescriptor vboDescriptor =
    {
        .vertexAttributeBindings = &vertexAttributeBinding,
        .vertexAttributeBindingSize = 1,
        .vertexAttribFormats = &vertexAttribFormat,
        .vertexAttribFormatsSize = 1,
        .vboId = m_FontQuadVbo.GetId()
    };
    
    m_FontQuadVao.ComputeDescriptor(vboDescriptor);
}

void GuiPass::ResetQuad() const
{
    const Vector4 vertices[6] = {
        // Vertex coordinates   // Texture coordinates
        { 0.5f,  0.5f,       1.0f, 1.0f}, 
        { 0.5f, -0.5f,       1.0f, 0.0f},  
        {-0.5f, -0.5f,       0.f,  0.0f},
        
        { 0.5f,  0.5f,       1.0f, 1.0f},
        {-0.5f, -0.5f,       0.0f, 0.0f}, 
        {-0.5f,  0.5f,       0.0f, 1.0f}
    };
    
    m_FontQuadVbo.UpdateData(0,sizeof(vertices),&vertices);
}

void GuiPass::RenderText() const
{
    m_FontQuadVao.BindBuffer();

    for (const TextComponent* textComponent : m_TextComponents)
    {
        if (!textComponent->enabled)
            continue;
        
        if (!textComponent->font.IsValid())
            continue;

        m_FontShader->SetVec3("textColor",static_cast<Vector3>(textComponent->color));
        float_t x = textComponent->screenTransform.x;
        const float_t y = textComponent->screenTransform.y;

        // update VBO for each character
        for (std::string::const_iterator c = textComponent->text.begin(); c != textComponent->text.end(); c++)
        {
            const Font::Character& ch = textComponent->font->GetGlyphByChar(*c);

            const Vector2 scale = textComponent->size * textComponent->fontSize;
            
            const float_t xPos = x + (static_cast<float_t>(ch.bearing.x) * scale.x);
            const float_t yPos = y - (static_cast<float_t>(ch.size.y - ch.bearing.y) * scale.y);

            const float_t w = (static_cast<float_t>(ch.size.x) * scale.x);
            const float_t h = (static_cast<float_t>(ch.size.y) * scale.y );
            
            const float_t vertices[6][4] = {
                { xPos,     yPos + h,   0.0f, 0.0f },            
                { xPos,     yPos,       0.0f, 1.0f },
                { xPos + w, yPos,       1.0f, 1.0f },

                { xPos,     yPos + h,   0.0f, 0.0f },
                { xPos + w, yPos,       1.0f, 1.0f },
                { xPos + w, yPos + h,   1.0f, 0.0f }           
            };

            ch.texture->BindTexture(0);
            
            m_FontQuadVbo.UpdateData(0, sizeof(vertices), vertices);
            
            Rhi::DrawArray(DrawMode::Triangles, 0, 6);

            x += static_cast<float_t>(ch.advance >> 6) * textComponent->fontSize;
        }
    }
    
    m_FontQuadVao.UnBindBuffer();
}


void GuiPass::RenderImage(const Vector2 viewPortSize) const
{
    for (const Image* image : m_Images)
        DrawImage(image, viewPortSize);
}

void GuiPass::DrawImage(const Image* imagComponent, Vector2 viewPortSize) const
{
    if (!imagComponent->enabled)
        return;
        
    if (!imagComponent->image.IsValid())
        return;

    imagComponent->image->BindTexture(0);
    Vector3 pos = static_cast<Vector3>(imagComponent->screenTransform * viewPortSize);
    pos.z = -1.f;
    pos.y = viewPortSize.y - pos.y; // Flip y axis to mach mouse position
    m_GuiShader->SetMat4("model",Matrix::Trs(pos,Quaternion::Identity(),static_cast<Vector3>(imagComponent->size)));

    // DRAW QUAD
    Rhi::DrawModel(DrawMode::Triangles,m_Quad->GetId());
}
