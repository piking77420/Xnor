#pragma once

#include "core.hpp"
#include "rendering/bloom_rendertarget.hpp"
#include "rendering/frame_buffer.hpp"
#include "rendering/render_pass.hpp"
#include "resource/compute_shader.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"

BEGIN_XNOR_CORE
class BloomPass
{
    static inline constexpr TextureInternalFormat::TextureInternalFormat BloomTextureFormat = TextureInternalFormat::TextureInternalFormat::Rgba32F;
    static inline constexpr float_t ComputeShaderDispactValue = 8;
public:
    DEFAULT_COPY_MOVE_OPERATIONS(BloomPass)

    XNOR_ENGINE BloomPass() = default;

    XNOR_ENGINE ~BloomPass() = default;

    XNOR_ENGINE void Init();
    
    XNOR_ENGINE void ComputeBloom(const Texture& imageWithoutBloom, const BloomRenderTarget& bloomRenderTarget) const;
private:
    Pointer<Model> m_Quad = nullptr;
    Pointer<ComputeShader> m_DownSample = nullptr;
    Pointer<ComputeShader> m_UpSample = nullptr;
    Pointer<ComputeShader> m_ThresholdFilter = nullptr;
    
    XNOR_ENGINE void UpSampling(const BloomRenderTarget& bloomRenderTarget) const;
    
    XNOR_ENGINE void DownSampling(const BloomRenderTarget& bloomRenderTarget) const;
    
    XNOR_ENGINE void ThresholdFilter(const Texture& imageWithoutBloom, const BloomRenderTarget& bloomRenderTarget) const;


};

END_XNOR_CORE
