#pragma once

#include <functional>

#include "core.hpp"
#include "audio/audio_buffer.hpp"
#include "audio/audio_device.hpp"

BEGIN_XNOR_CORE

class Audio
{
    STATIC_CLASS(Audio)

public:
    XNOR_ENGINE static bool_t Initialize();

    XNOR_ENGINE static void Shutdown();

    /// @brief Returns the current device's context.
    [[nodiscard]]
    XNOR_ENGINE static AudioContext* GetContext();

    XNOR_ENGINE static void RegisterBuffer(AudioBuffer* buffer);

    XNOR_ENGINE static void UnregisterBuffer(AudioBuffer* buffer);

private:
    XNOR_ENGINE static inline List<AudioDevice*> m_AvailableDevices;
    XNOR_ENGINE static inline AudioDevice* m_CurrentDevice = nullptr;

    XNOR_ENGINE static inline List<AudioBuffer*> m_Buffers;

    XNOR_ENGINE static void InitializeDevices();

    /// @brief Iterates over an OpenAL string list, applying the given lambda on each string.
    ///
    /// OpenAL string lists are strings separated by null characters, and ending with two consecutive null characters.
    XNOR_ENGINE static void IterateAlStringList(const char_t* list, const std::function<void(const char_t*)>& lambda);
};

END_XNOR_CORE
