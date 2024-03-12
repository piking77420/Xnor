#pragma once

#include "core.hpp"
#include "jolt/Jolt.h"
#include "Jolt/Physics/Collision/ContactListener.h"

BEGIN_XNOR_CORE

class XNOR_ENGINE ContactListenerImpl : public JPH::ContactListener
{
    JPH::ValidateResult OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult) override;
    void OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override;
    void OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override;
    void OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override;
};

END_XNOR_CORE
