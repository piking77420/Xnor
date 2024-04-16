#pragma once

#include "core.hpp"
#include "resource/audio_track.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE

class AudioSource : public Component
{
    REFLECTABLE_IMPL(AudioSource)
    
public:
    Pointer<AudioTrack> track;
    
    XNOR_ENGINE AudioSource();
    
    XNOR_ENGINE ~AudioSource() override;

    DEFAULT_COPY_MOVE_OPERATIONS(AudioSource)
    
    XNOR_ENGINE void Begin() override;
    
    XNOR_ENGINE void Update() override;

    XNOR_ENGINE void Play(AudioTrack& track);

    XNOR_ENGINE void SetBuffer(const AudioBuffer* buffer);

    XNOR_ENGINE bool_t GetLooping() const;

    XNOR_ENGINE void SetLooping(bool_t looping);

private:
    uint32_t m_Handle = 0;

    AudioContext* m_Context = nullptr;
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::AudioSource, bases<XnorCore::Component>),
    field(track)
)
