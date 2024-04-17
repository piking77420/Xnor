#include "audio/component/audio_listener.hpp"

#include <AL/al.h>

#include "transform.hpp"
#include "audio/audio.hpp"

using namespace XnorCore;

AudioListener::AudioListener() { m_Context = Audio::GetContext(); }

void AudioListener::Update()
{
    m_Context->MakeCurrent();

    const Transform& transform = GetTransform();

    // Position
    alListenerfv(AL_POSITION, transform.GetPosition().Raw());
    AudioContext::CheckError();

    Vector3 velocity;
    std::array orientation = { transform.worldMatrix * Vector3::UnitZ(), Vector3::UnitY() };
    if (!dopplerEffect)
    {
        velocity = Vector3::Zero();
        orientation[0] = Vector3::Zero();
    }
    
    // Velocity
    alListenerfv(AL_VELOCITY, velocity.Raw());
    AudioContext::CheckError();

    // Orientation
    alListenerfv(AL_ORIENTATION, orientation[0].Raw());
    AudioContext::CheckError();
}

float_t AudioListener::GetVolume() const
{
    return m_Volume;
}

void AudioListener::SetVolume(const float_t newVolume)
{
    m_Volume = std::max(0.f, newVolume);
    
    m_Context->MakeCurrent();
    alListenerf(AL_GAIN, m_Volume);
    AudioContext::CheckError();
}
