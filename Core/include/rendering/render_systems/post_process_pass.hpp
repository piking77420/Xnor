#pragma once

#include "bloom_pass.hpp"
#include "core.hpp"
#include "tone_mapping.hpp"
#include "rendering/frame_buffer.hpp"
#include "resource/compute_shader.hpp"

BEGIN_XNOR_CORE
    class PostProcessPass
{
public:
    bool enable = true;
    
    XNOR_ENGINE PostProcessPass() = default;
    
    XNOR_ENGINE ~PostProcessPass();
    
    XNOR_ENGINE void Init();
    
    XNOR_ENGINE void Compute(const Texture& textureToCompute, const Texture& PostProcessedTexture);

private:
    XNOR_ENGINE void HandleResize(Vector2i size);
    
    FrameBuffer* m_FrameBuffer = nullptr;
    ToneMapping m_ToneMapping;
    BloomPass m_BloomPass;
    RenderPass m_RenderPass;

    static inline Pointer<ComputeShader> m_TestCompute = nullptr;

};

END_XNOR_CORE;
