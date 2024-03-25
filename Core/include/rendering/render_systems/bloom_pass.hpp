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
private:
    static inline Pointer<Model> m_Quad = nullptr;
    static inline Pointer<Shader> m_DownSample = nullptr;
    static inline Pointer<ComputeShader> m_UpSample = nullptr;
    static inline Pointer<Shader> m_ThresholdFilter = nullptr;
    static inline Pointer<Shader> m_TestShaderCompute = nullptr;
    static inline constexpr TextureInternalFormat::TextureInternalFormat BloomTextureFormat = TextureInternalFormat::TextureInternalFormat::Rgba32F;
    
public:
    DEFAULT_COPY_MOVE_OPERATIONS(BloomPass)

    XNOR_ENGINE BloomPass() = default;

    XNOR_ENGINE ~BloomPass() = default;

    void Init();
    
    XNOR_ENGINE void ComputeBloom(const Texture& imageWithoutBloom, const BloomRenderTarget& bloomRenderTarget) const;
private:
    float_t m_FilterRadius = 0.005f;
    
    XNOR_ENGINE void UpSampling(const BloomRenderTarget& bloomRenderTarget) const;
    
    XNOR_ENGINE void DownSampling(const BloomRenderTarget& bloomRenderTarget) const;
    
    XNOR_ENGINE void ThresholdFilter(const Texture& imageWithoutBloom, const BloomRenderTarget& bloomRenderTarget) const;


};

END_XNOR_CORE
