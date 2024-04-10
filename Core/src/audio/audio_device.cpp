#include "audio/audio_device.hpp"

#include <AL/alc.h>

#include "utils/logger.hpp"

using namespace XnorCore;

bool_t AudioDevice::CheckError(const AudioDevice* device)
{
    ALCdevice* const handle = device ? device->m_Handle : nullptr;
    
    const ALCenum error = alcGetError(handle);

    if (error != ALC_NO_ERROR)
    {
        Logger::LogError("[OpenAL] {}", std::string_view(alcGetString(handle, error)));
        return true;
    }

    return false;
}

AudioDevice::AudioDevice(std::string&& name)
    : m_Handle(alcOpenDevice(name.c_str()))
    , m_Name(std::move(name))
{
    if (!m_Handle || CheckError(this))
    {
        Logger::LogError("Unable to open audio device {}", m_Name);
        return;
    }

    m_Context = new AudioContext(*this);

    DEBUG_LOG("Max mono source count: {}", m_Context->GetMaxSourceCount(AudioSourceType::Mono));
    DEBUG_LOG("Max stereo source count: {}", m_Context->GetMaxSourceCount(AudioSourceType::Stereo));
}

AudioDevice::~AudioDevice()
{
    delete m_Context;
    alcCloseDevice(m_Handle);
}

std::string AudioDevice::GetName() const { return m_Name; }
