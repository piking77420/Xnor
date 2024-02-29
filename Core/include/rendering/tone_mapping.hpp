#pragma once

#include <Maths/vector2i.hpp>

#include "core.hpp"
#include "frame_buffer.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"

BEGIN_XNOR_CORE
class ToneMapping
{
public:
    XNOR_ENGINE ToneMapping() = default;
    
    XNOR_ENGINE ~ToneMapping() = default;

    XNOR_ENGINE void InitializeResources();
    
    XNOR_ENGINE void Initialize(Vector2i windowSize);
    
    XNOR_ENGINE void Destroy() const;

    XNOR_ENGINE void RecompileShader();
    
    XNOR_ENGINE void OnResizeWindow(Vector2i windowSize);

    XNOR_ENGINE void ComputeToneMaping(const Texture& imageWithoutToneMapping, const Pointer<Model>& cubeMode) const;

    XNOR_ENGINE const Texture& GetToneMapedImage() const;
    
private:
    Pointer<Shader> m_Aces = nullptr;

    FrameBuffer* m_FrameBuffer = nullptr;
    Texture* m_ColorAttachment = nullptr;
};

END_XNOR_CORE
