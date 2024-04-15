#pragma once

#include "core.hpp"
#include "audio/audio_context.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE

class AudioListener : public Component
{
public:
    bool_t dopplerEffect = true;
    
    AudioListener();
    
    ~AudioListener() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(AudioListener)
    
    void Update() override;
    
    /// @brief Get the volume in the range [0, inf].
    [[nodiscard]]
    float_t GetVolume() const;
    
    /// @brief Set the volume in the range [0, inf]. Default value is 1.
    void SetVolume(float_t newVolume);

private:
    AudioContext* m_AudioContext = nullptr;
    
    /// @brief Volume in the range [0, inf]. Default value is 1.
    float_t volume = 1.f;
};

END_XNOR_CORE
