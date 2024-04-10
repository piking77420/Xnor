#include "audio/audio_context.hpp"

#include <AL/alc.h>

#include "audio/audio_device.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

AudioContext::AudioContext(AudioDevice& device)
    : m_Handle(alcCreateContext(device.m_Handle, nullptr))
    , m_Device(&device)
{
    if (!m_Handle || AudioDevice::CheckError(m_Device))
    {
        Logger::LogError("Unable to create audio context for device {}", device.GetName());
        return;
    }
    
    if (!alcMakeContextCurrent(m_Handle) || AudioDevice::CheckError(m_Device))
    {
        Logger::LogError("Unable to make audio context current for device {}", device.GetName());
        return;
    }

    // Get the context attribute values
    int32_t size = 0;
    alcGetIntegerv(m_Device->m_Handle, ALC_ATTRIBUTES_SIZE, 1, &size);
    m_Attributes.resize(size);
    alcGetIntegerv(m_Device->m_Handle, ALC_ALL_ATTRIBUTES, size, m_Attributes.data());
}

AudioContext::~AudioContext()
{
    alcDestroyContext(m_Handle);
    AudioDevice::CheckError(m_Device);
}

int32_t AudioContext::GetMaxSourceCount(const AudioSourceType::AudioSourceType sourceType) const
{
    int32_t result = 0;
    
    for (size_t i = 0; i < m_Attributes.size(); i++)
    {
        if ((sourceType == AudioSourceType::Mono && m_Attributes[i] == ALC_MONO_SOURCES) || (sourceType == AudioSourceType::Stereo && m_Attributes[i] == ALC_STEREO_SOURCES))
            result += m_Attributes[i + 1];
    }

    return result;
}
