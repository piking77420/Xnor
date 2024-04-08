#include "audio/audio_device.hpp"

#include "AL/alc.h"

using namespace XnorCore;

AudioDevice::AudioDevice(const std::string& name)
{
    alcOpenDevice()
}
