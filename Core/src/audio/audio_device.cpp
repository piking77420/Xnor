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
}

AudioDevice::~AudioDevice()
{
    delete m_Context;
    alcCloseDevice(m_Handle);
}

AudioContext* AudioDevice::GetContext() { return m_Context; }

const AudioContext* AudioDevice::GetContext() const { return m_Context; }

std::string AudioDevice::GetName() const { return m_Name; }
