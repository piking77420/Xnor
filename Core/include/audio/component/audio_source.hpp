#pragma once

#include "core.hpp"
#include "resource/audio_track.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE AudioSource : public Component
{
    REFLECTABLE_IMPL(AudioSource)
    
public:
    AudioSource();
    
    ~AudioSource() override;

    DEFAULT_COPY_MOVE_OPERATIONS(AudioSource)
    
    void Begin() override;
    
    void Update() override;

    void Play(AudioTrack& track);

    void SetBuffer(const AudioBuffer* buffer);

    bool_t GetLooping() const;

    void SetLooping(bool_t looping);

private:
    uint32_t m_Handle = 0;

    AudioContext* m_Context = nullptr;
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::AudioSource, bases<XnorCore::Component>)
)
