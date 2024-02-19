#pragma once

#include "core.hpp"
#include "Maths/vector4.hpp"
#include "renderer_context.hpp"
#include "file/file_manager.hpp"
#include "scene/scene.hpp"
#include "rendering/rhi.hpp"
#include "resource/model.hpp"

BEGIN_XNOR_CORE

class Renderer
{
public:
    Vector4 clearColor;

    XNOR_ENGINE explicit Renderer();
    
    XNOR_ENGINE void RenderScene(const Scene& scene, const RendererContext& rendererContext) const;

    XNOR_ENGINE void CompileShader();

private:
    RHI m_Rhi;

    Pointer<File> m_VertexPath;
    Pointer<File> m_FragmentPath;
    Pointer<Texture> m_Diamondtexture;
    Pointer<File> m_DiamondPath;

    Pointer<Shader> m_BasicShader;
    Pointer<Model> m_Model;
};

END_XNOR_CORE
