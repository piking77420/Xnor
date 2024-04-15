#include "resource/audio_track.hpp"

#include "utils/logger.hpp"

using namespace XnorCore;

AudioTrack::~AudioTrack()
{
    AudioTrack::Unload();
}

bool_t AudioTrack::Load(const uint8_t* buffer, const int64_t length)
{
    const char_t* const type = reinterpret_cast<const char_t*>(buffer);

    if (std::strncmp(type, "RIFF", 4) == 0)
        return LoadWavefront(buffer, length);

    Logger::LogError("Unsupported file format {}", std::string_view(type, 4));
    return false;
}

void AudioTrack::CreateInInterface()
{
    Resource::CreateInInterface();
}

void AudioTrack::DestroyInInterface()
{
    Resource::DestroyInInterface();
}

void AudioTrack::Unload()
{
    delete m_Data;
}

bool_t AudioTrack::LoadWavefront(const uint8_t* const buffer, const int64_t length)
{
    int64_t offset = 8; // 4 because 'RIFF' has already been checked and 4 because of the file size that we don't need
    const char_t* const str = reinterpret_cast<const char_t*>(buffer);
    
    if (std::strncmp(str + offset, "WAVE", 4) != 0)
        return false;
    offset += 4;

    while (offset < length)
    {
        if (strncmp(str + offset, "fmt ", 4) != 0)
            offset += LoadWavefrontFormat(buffer + offset);
        else if (strncmp(str + offset, "data", 4) != 0)
            offset += LoadWavefrontData(buffer + offset);
        else
            offset++;
    }
    
    return true;
}

int64_t AudioTrack::LoadWavefrontFormat(const uint8_t* const data)
{
    int64_t offset = 8; // 4 because 'fmt ' has already been checked and 4 because of the chunk size that we don't need
    
    offset += sizeof(uint16_t); // Skip the format type - we don't care about it
    
    m_Channels = *reinterpret_cast<const decltype(m_Channels)*>(data + offset);
    offset += sizeof(m_Channels);
    
    m_SampleRate = *reinterpret_cast<const decltype(m_SampleRate)*>(data + offset);
    offset += sizeof(m_SampleRate);

    offset += sizeof(uint32_t); // Skip whatever is this value

    offset += sizeof(uint16_t); // As well as this one
    
    m_BitDepth = *reinterpret_cast<const decltype(m_BitDepth)*>(data + offset);
    offset += sizeof(m_BitDepth);

    return offset;
}

int64_t AudioTrack::LoadWavefrontData(const uint8_t* const data)
{
    const uint32_t length = *reinterpret_cast<const uint32_t*>(data + 4);
    m_Data = data + 8;
    m_Size = length;

    return length + 8;
}
