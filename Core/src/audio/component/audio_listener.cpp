#include "audio/component/audio_listener.hpp"

#include <AL/al.h>

#include "transform.hpp"
#include "audio/audio.hpp"

using namespace XnorCore;

AudioListener::AudioListener() { m_AudioContext = Audio::GetContext(); }

void AudioListener::Update()
{
    m_AudioContext->MakeCurrent();

    const Transform& transform = GetTransform();

    // Position
    alListenerfv(AL_POSITION, transform.GetPosition().Raw());
    AudioContext::CheckError();

    // TODO: Velocity

    // Orientation
    std::array at = { transform.worldMatrix * Vector3::UnitZ(), Vector3::UnitY() };
    alListenerfv(AL_ORIENTATION, at[0].Raw());
    AudioContext::CheckError();
}

float_t AudioListener::GetVolume() const
{
    return m_Volume;
}

void AudioListener::SetVolume(const float_t newVolume)
{
    m_Volume = std::max(0.f, newVolume);
    
    m_AudioContext->MakeCurrent();
    alListenerf(AL_GAIN, m_Volume);
    AudioContext::CheckError();
}
