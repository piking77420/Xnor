#pragma once

#include "bloom_pass.hpp"
#include "core.hpp"
#include "tone_mapping.hpp"
#include "rendering/frame_buffer.hpp"
#include "rendering/postprocess_rendertarget.hpp"
#include "resource/compute_shader.hpp"

BEGIN_XNOR_CORE
class PostProcessPass
{
public:
    XNOR_ENGINE PostProcessPass() = default;
    
    XNOR_ENGINE ~PostProcessPass();
    
    XNOR_ENGINE void Init();
    
    XNOR_ENGINE void Compute(const Texture& textureToCompute, const Texture& postProcessedTexture,const PostprocessRendertarget& postprocessRendertarget) const;

private:
    FrameBuffer* m_FrameBuffer = nullptr;
    ToneMapping m_ToneMapping;
    BloomPass m_BloomPass;
    RenderPass m_RenderPass;
};

END_XNOR_CORE;
