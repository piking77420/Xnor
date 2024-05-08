﻿#pragma once
#include "core.hpp"
#include "physics/components/collider.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE
class PlayerMovment : public Component
{
private:
    REFLECTABLE_IMPL(PlayerMovment)

public:
    float_t speed = 1.f;

    Collider* collider;
    
    DEFAULT_COPY_MOVE_OPERATIONS(PlayerMovment)

    PlayerMovment() = default;

    ~PlayerMovment() = default;

    void Update() override;

    void Begin() override;
private:
    void Movement();
    
    void CameraMovement();
};

END_XNOR_CORE
/// @private
REFL_AUTO(
    type(XnorCore::PlayerMovment, bases<XnorCore::Component>),
    field(speed)
);