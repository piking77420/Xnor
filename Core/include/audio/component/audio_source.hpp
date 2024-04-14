#pragma once

#include "core.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE

class AudioSource : public Component
{
public:
    ~AudioSource() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(AudioSource)
    
    void Begin() override;
    void Update() override;
};

END_XNOR_CORE
