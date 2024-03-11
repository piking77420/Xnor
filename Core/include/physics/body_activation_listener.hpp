#pragma once

#include "core.hpp"
#include "Jolt/Core/Core.h"
#include "jolt/Physics/Body/BodyActivationListener.h"

BEGIN_XNOR_CORE

class BodyActivationListenerImpl : public JPH::BodyActivationListener
{
public:
    void OnBodyActivated(const JPH::BodyID& inBodyId, JPH::uint64 inBodyUserData) override;

    void OnBodyDeactivated(const JPH::BodyID& inBodyId, JPH::uint64 inBodyUserData) override;
};

END_XNOR_CORE
