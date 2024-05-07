#include "rendering/render_systems/font_pass.hpp"

#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"
#include "scene/component/texte_component.hpp"

using namespace XnorCore;


void FontPass::InitResources()
{
    m_FontShader = ResourceManager::Get<Shader>("font_shader");

    BlendFunction blendFunction = {.isBlending = true, .sValue = BlendValue::SrcColor, .dValue = BlendValue::OneMinusSrcAlpha, .blendEquation = BlendEquation::Add};
    
    m_FontShader->SetBlendFunction(blendFunction);
    m_FontShader->CreateInInterface();
    
    m_FontShader->Use();
    m_FontShader->SetInt("text",CharBindIndex);
    m_FontShader->Unuse();

    m_vaoQuad.BindBuffer();
    
    
    m_vboQuad.Allocate( sizeof(float) * 6 * 4, nullptr, BufferUsage::DynamicDraw);
    VertexAttributePointer attribute_pointer =
        {
            .index = 0,
            .size = 4,
            .bufferDatatype = DataType::Float,
            .normalized = false,
            .stride = 4 * sizeof(float_t),
            .pointer = nullptr
        };
    
    VBODescriptor vboDescriptor =
        {
            .vertexAttributesPointer = &attribute_pointer,
            .vboAttributePointerSize = 1,
            .attributesDivisorPointer = nullptr,
            .attributesDivisorPointerSize = 0
        };
    m_vboQuad.UnBind();

    m_vaoQuad.UnBindBuffer();
}

void FontPass::RenderFont(const Scene& scene, const Viewport& viewport) const
{
    std::vector<const TexteComponent*> textComponents;
    scene.GetAllComponentOfType<TexteComponent>(&textComponents);

    const Vector2i screenSize = viewport.viewPortSize;
    
    m_FontShader->Use();

    for (const TexteComponent* texteComponent : textComponents)
    {
        const std::string& text = texteComponent->text;
        m_vaoQuad.BindBuffer();
        Vector2 charPos = texteComponent->screenSpaceTransform; 
        
        for (const char_t c : text)
        {
            const Font::Character& character = texteComponent->font->GetGlyphByChar(c);
            
            const float_t xpos = charPos.x + character.bearing.x * texteComponent->police;
            const float_t ypos = charPos.y - (character.size.y - character.size.y) * texteComponent->police;
            const float_t w = character.size.x * texteComponent->police;
            const float_t h = character.size.y * texteComponent->police;
            
            float_t vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },            
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }           
            };
            character.texture.BindTexture(CharBindIndex);
            m_vboQuad.UpdateData(0, sizeof(vertices),vertices);
            
            m_FontShader->SetVec3("color",static_cast<Vector3>(texteComponent->color));
            Matrix ortho = Camera::Ortho(0.f,screenSize.x,0.f,screenSize.y,0.1f,1000.f);
            m_FontShader->SetMat4("projection",ortho);
            
            
            
            Rhi::DrawArray(DrawMode::Triangles,0,6);
            
            character.texture.UnbindTexture(CharBindIndex);
            charPos.x += (character.advance >> 6) * texteComponent->police; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))

        }
    }
    
    m_vaoQuad.UnBindBuffer();
    m_FontShader->Unuse();   
}
