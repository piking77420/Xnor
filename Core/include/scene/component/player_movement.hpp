#pragma once

#include "core.hpp"
#include "physics/components/collider.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE

class PlayerMovement : public Component
{
    REFLECTABLE_IMPL(PlayerMovement)

public:
    float_t speed = 1.f;

    Collider* collider;
    
    DEFAULT_COPY_MOVE_OPERATIONS(PlayerMovement)

    PlayerMovement() = default;

    ~PlayerMovement() override = default;

    void Update() override;

    void Begin() override;
    
private:
    void Movement();
    
    void CameraMovement();
};

END_XNOR_CORE

/// @private
REFL_AUTO(
    type(XnorCore::PlayerMovement, bases<XnorCore::Component>),
    field(speed)
);
