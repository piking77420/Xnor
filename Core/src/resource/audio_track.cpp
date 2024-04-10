#include "resource/audio_track.hpp"

using namespace XnorCore;

AudioTrack::~AudioTrack()
{
    AudioTrack::Unload();
}

bool_t AudioTrack::Load(const uint8_t* buffer, const int64_t length)
{
    return Resource::Load(buffer, length);
}

void AudioTrack::Unload()
{
    delete m_Data;
}

bool_t AudioTrack::LoadWavefront()
{
    return true;
}
