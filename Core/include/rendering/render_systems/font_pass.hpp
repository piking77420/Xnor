#pragma once

#include <rendering/buffer/vbo.hpp>

#include "core.hpp"
#include "rendering/buffer/vao.hpp"
#include "resource/shader.hpp"
#include "scene/scene.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_CORE
class FontPass
{
private:
    static constexpr uint32_t CharBindIndex = 0;
public:

    DEFAULT_COPY_MOVE_OPERATIONS(FontPass)

    FontPass() = default;

    ~FontPass() = default;

    XNOR_ENGINE void InitResources();
    
    XNOR_ENGINE void RenderFont(const Scene& scene, const Viewport& viewport) const;

private:
    Pointer<Shader> m_FontShader;

    VAO m_vaoQuad;
    mutable VBO m_vboQuad;
};

END_XNOR_CORE