#pragma once

#include <string>

#include "core.hpp"

// ReSharper disable once CppInconsistentNaming
struct ALCdevice;

BEGIN_XNOR_CORE

class AudioDevice
{
public:
    [[nodiscard]]
    explicit AudioDevice(const std::string& name);

private:
    ALCdevice* m_Handle = nullptr;
};

END_XNOR_CORE
