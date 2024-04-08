#pragma once

#include "core.hpp"

BEGIN_XNOR_CORE

class Audio
{
    STATIC_CLASS(Audio)

public:
    [[nodiscard]]
    XNOR_ENGINE static bool_t Initialize();

    XNOR_ENGINE static void Shutdown();
};

END_XNOR_CORE
