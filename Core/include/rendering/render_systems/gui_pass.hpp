#pragma once
#include <rendering/buffer/vbo.hpp>

#include "core.hpp"
#include "rendering/buffer/vao.hpp"
#include "resource/shader.hpp"
#include "scene/scene.hpp"
#include "scene/component/button.hpp"
#include "scene/component/texte_component.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_CORE
class GuiPass
{
public:
    XNOR_ENGINE GuiPass() = default;

    XNOR_ENGINE ~GuiPass() = default;
    
    XNOR_ENGINE void RenderGui(const Scene& scene,const Vector2i& viewPortSizei, float_t aspect) const;

    XNOR_ENGINE void Init();
    
private:
    Pointer<Shader> m_GuiShader;

    Pointer<Shader> m_FontShader;

    Pointer<Model> m_Quad;

    mutable VAO m_FontQuadVao;
    
    mutable VBO m_FontQuadVbo;
    
    mutable std::vector<const Image*> m_Images;
    
    mutable std::vector<const TexteComponent*> m_TextComponents;

    XNOR_ENGINE void InitSliderQuad();

    XNOR_ENGINE void ResetQuad() const;

    XNOR_ENGINE void RenderText(const Vector2 viewPortSize, float_t aspect) const;
    
    XNOR_ENGINE void RenderImage(Vector2 viewPortSize) const;
    
    XNOR_ENGINE void DrawImage(const Image* imagComponent,Vector2 viewPortSize) const;
};

END_XNOR_CORE