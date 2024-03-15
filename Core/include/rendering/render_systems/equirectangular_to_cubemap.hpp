#pragma once

#include "core.hpp"
#include "rendering/render_pass.hpp"
#include "resource/cubemap.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"

BEGIN_XNOR_CORE
class EquirectangularToCubeMap
{

public:
    XNOR_ENGINE EquirectangularToCubeMap() = default;
    
    XNOR_ENGINE  ~EquirectangularToCubeMap();

    XNOR_ENGINE void EquirectangularToCubeMapFunc(const Texture& equirectangularMap,const Cubemap& cubemap);

    XNOR_ENGINE void ComputeIradiance(const Texture& equirectangularMap,const Cubemap& cubemap);

    XNOR_ENGINE void InitResource();

private:
    Pointer<Model> m_Cube;
    FrameBuffer* m_FrameBuffer = nullptr;
    RenderPass m_RenderPass;
    Pointer<Shader> m_EquirectangularToCubeMapShader;

    Texture* m_TextureDepth = nullptr;
    Texture* m_color = nullptr;
    
    XNOR_ENGINE void InitCreateFrameBuffer(Vector2i size);
    
    XNOR_ENGINE void Compute(const Texture& textureToBind,const Cubemap& cubemap, const Pointer<Shader>& shader);
};

END_XNOR_CORE
