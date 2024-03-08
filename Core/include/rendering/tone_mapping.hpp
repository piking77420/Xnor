#pragma once

#include "core.hpp"
#include "frame_buffer.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"

/// @file tone_mapping.hpp
/// Defines the XnorCore::ToneMapping class

BEGIN_XNOR_CORE

/// @brief Encapsulates the ACES tone mapping algorithm
class ToneMapping
{
public:
    XNOR_ENGINE ToneMapping() = default;
    
    XNOR_ENGINE ~ToneMapping() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(ToneMapping)

    /// @brief Initializes the shader resources
    XNOR_ENGINE void InitializeResources();
    
    /// @brief Prepares the tone mapping
    /// @param windowSize Window size
    XNOR_ENGINE void Prepare(Vector2i windowSize);

    /// @brief Destroys the tone mapping resources
    XNOR_ENGINE void Destroy() const;

    /// @brief Recompiles the underlying shader
    XNOR_ENGINE void RecompileShader();

    /// @brief Handles a window resize event
    /// @param windowSize Window size
    XNOR_ENGINE void OnResizeWindow(Vector2i windowSize);

    /// @brief Computes the tone mapping
    /// @param imageWithoutToneMapping Initial image
    /// @param quadModel Quad model
    XNOR_ENGINE void ComputeToneMaping(const Texture& imageWithoutToneMapping, const Pointer<Model>& quadModel) const;

    /// @brief Gets the resulting tone mapped image
    /// @return Resulting image
    XNOR_ENGINE const Texture& GetToneMapedImage() const;
    
private:
    Pointer<Shader> m_Aces = nullptr;

    FrameBuffer* m_FrameBuffer = nullptr;
    Texture* m_ColorAttachment = nullptr;
};

END_XNOR_CORE
