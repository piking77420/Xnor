#pragma once

#include "core.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE

class AudioListener : public Component
{
public:
    ~AudioListener() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(AudioListener)
    
    void Begin() override;
    void Update() override;
};

END_XNOR_CORE
