#pragma once

#include "core.hpp"
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
    static inline Pointer<Shader> m_TresholdFilter = nullptr;
    static inline Pointer<Shader> m_TestShaderCompute = nullptr;
    
public:
    DEFAULT_COPY_MOVE_OPERATIONS(BloomPass)

    XNOR_ENGINE BloomPass() = default;

    XNOR_ENGINE ~BloomPass() = default;
    
    XNOR_ENGINE void Init(uint32_t bloomMips);
    
    XNOR_ENGINE void ComputeBloom(const Texture& textureWithoutBloom);

    XNOR_ENGINE Texture* GetBloomTexture() const;

private:
    struct BloomMip
    {
        Texture* texture;
        Vector2 sizef;
    };
    
    FrameBuffer* m_FrameBuffer = nullptr;
    RenderPass m_RenderPass;
    std::vector<BloomMip> m_MipChain;
    Texture* m_ThresholdTexture = nullptr;

    uint32_t m_BloomMips{};

    float_t m_FilterRadius = 0.005f;


    XNOR_ENGINE void UpSampling();
    
    XNOR_ENGINE void DownSampling();

    XNOR_ENGINE void HandleBlooMip(Vector2i currentViewPortSize);

    XNOR_ENGINE void ThresholdFilter(const Texture& textureToCompute);


};

END_XNOR_CORE
