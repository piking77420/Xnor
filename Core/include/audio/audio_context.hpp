#pragma once

#include <vector>

#include "core.hpp"

// ReSharper disable once CppInconsistentNaming
struct ALCcontext;

BEGIN_XNOR_CORE

class AudioDevice;

BEGIN_ENUM(AudioSourceType)
{
    Mono,
    Stereo
}
END_ENUM

class AudioContext
{
public:
    [[nodiscard]]
    explicit AudioContext(AudioDevice& device);

    ~AudioContext();

    DEFAULT_COPY_MOVE_OPERATIONS(AudioContext)

    int32_t GetMaxSourceCount(ENUM_VALUE(AudioSourceType) sourceType) const;

private:
    ALCcontext* m_Handle = nullptr;
    AudioDevice* m_Device = nullptr;

    std::vector<int32_t> m_Attributes;
};

END_XNOR_CORE
