#pragma once

#include "core.hpp"
#include "resource/resource.hpp"

BEGIN_XNOR_CORE

class AudioTrack : public Resource
{
public:
    /// @brief Allowed extensions for audio files
    XNOR_ENGINE static inline constexpr std::array<const char_t*, 3> FileExtensions
    {
        ".wav",
        ".mp3",
        ".ogg"
    };
    
    // Same constructor from base class
    using Resource::Resource;

    ~AudioTrack();
    
    bool_t Load(const uint8_t* buffer, int64_t length) override;
    
    XNOR_ENGINE void Unload() override;

    /// @brief Gets the raw data of the texture
    /// @tparam T Type
    /// @return Data
    template<typename T = char_t>
    [[nodiscard]]
    const T* GetData() const;

    /// @brief Gets the raw data of the texture
    /// @tparam T Type
    /// @return Data
    template<typename T = char_t>
    [[nodiscard]]
    T* GetData();

private:
    uint8_t* m_Data = nullptr;
    uint64_t m_Size;
    
    bool_t LoadWavefront();
};

END_XNOR_CORE

#include "resource/audio_track.inl"
