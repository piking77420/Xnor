#include "audio/component/audio_source.hpp"

#include <AL/al.h>

#include "transform.hpp"
#include "audio/audio.hpp"
#include "input/input.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

AudioSource::AudioSource()
    : m_Context(Audio::GetContext())
{
    m_Context->MakeCurrent();
    alGenSources(1, &m_Handle);
}

AudioSource::~AudioSource()
{
    m_Context->MakeCurrent();
    alDeleteSources(1, &m_Handle);
}

void AudioSource::Begin()
{
    SetLooping(true);
    Play(*track);
}

void AudioSource::Update()
{
    m_Context->MakeCurrent();

    const Transform& transform = GetTransform();
    
    // Position
    alSourcefv(m_Handle, AL_POSITION, transform.GetPosition().Raw());
    AudioContext::CheckError();

    // TODO: Velocity

    // Direction
    alSourcefv(m_Handle, AL_DIRECTION, (transform.worldMatrix * Vector3::UnitZ()).Raw());
    AudioContext::CheckError();
}

void AudioSource::Play(AudioTrack& track)
{
    if (!track.IsLoadedInInterface())
    {
        Logger::LogWarning("Tried to play an AudioTrack ({}) that wasn't loaded in the audio interface. Loading it", track.GetName());
        track.CreateInInterface();
    }

    SetBuffer(track.GetBuffer());
    alSourcePlay(m_Handle);
    AudioContext::CheckError();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AudioSource::SetBuffer(const AudioBuffer* buffer)
{
    m_Context->MakeCurrent();
    alSourcei(m_Handle, AL_BUFFER, static_cast<int32_t>(buffer->GetHandle()));
    AudioContext::CheckError();
}

bool_t AudioSource::GetLooping() const
{
    int32_t result = 0;
    m_Context->MakeCurrent();
    alGetSourcei(m_Handle, AL_LOOPING, &result);
    AudioContext::CheckError();
    return static_cast<bool_t>(result);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AudioSource::SetLooping(const bool_t looping)
{
    m_Context->MakeCurrent();
    alSourcei(m_Handle, AL_LOOPING, looping);
    AudioContext::CheckError();
}
