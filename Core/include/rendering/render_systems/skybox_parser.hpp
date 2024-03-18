#pragma once

#include "core.hpp"
#include "rendering/render_pass.hpp"
#include "resource/cubemap.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"

BEGIN_XNOR_CORE

class SkyBoxParser
{
public:
    XNOR_ENGINE SkyBoxParser() = default;
    
    XNOR_ENGINE ~SkyBoxParser();

    DEFAULT_COPY_MOVE_OPERATIONS(SkyBoxParser)

    XNOR_ENGINE void EquirectangularToCubeMapFunc(const Texture& equirectangularMap,const Cubemap& cubemap);

    XNOR_ENGINE void ComputeIrradiance(const Cubemap& irradianceInput,const Cubemap& irradianceOutput, Vector2i irradianceSize);
    
    XNOR_ENGINE void ComputePreFiltering(const Cubemap& environementMap, const Cubemap& prefilteringMap, uint32_t mipLevels);

    XNOR_ENGINE void PreComputeBrdf(Vector2i environementMapSize, const Texture& brdfTexture);
    
    XNOR_ENGINE void InitResource();

private:
    Pointer<Model> m_Cube;
    Pointer<Model> m_Quad;

    FrameBuffer* m_FrameBuffer = nullptr;
    RenderPass m_RenderPass;
    Pointer<Shader> m_EquirectangularToCubeMapShader;
    Pointer<Shader> m_IrradianceConvolution;
    
    Pointer<Shader> m_PrefilterShader;
    Pointer<Shader> m_PreComputeBrdr;

    Texture* m_TextureDepth = nullptr;
    Texture* m_Color = nullptr;
    
    XNOR_ENGINE void IsFrameBufferValid(Vector2i size);
    
    XNOR_ENGINE void Compute(const Texture& equirectangularMap, const Cubemap& cubemap, const Pointer<Shader>& shader);
};

END_XNOR_CORE
