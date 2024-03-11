#include "physics/contact_listener.hpp"

using namespace XnorCore;


JPH::ValidateResult ContactListenerImpl::OnContactValidate(
    const JPH::Body& inBody1,
    const JPH::Body& inBody2,
    const JPH::RVec3Arg inBaseOffset,
    const JPH::CollideShapeResult& inCollisionResult
)
{
    return ContactListener::OnContactValidate(inBody1, inBody2, inBaseOffset, inCollisionResult);
}

void ContactListenerImpl::OnContactAdded(
    const JPH::Body& inBody1,
    const JPH::Body& inBody2,
    const JPH::ContactManifold& inManifold,
    JPH::ContactSettings& ioSettings
)
{
    ContactListener::OnContactAdded(inBody1, inBody2, inManifold, ioSettings);
}

void ContactListenerImpl::OnContactPersisted(
    const JPH::Body& inBody1,
    const JPH::Body& inBody2,
    const JPH::ContactManifold& inManifold,
    JPH::ContactSettings& ioSettings
)
{
    ContactListener::OnContactPersisted(inBody1, inBody2, inManifold, ioSettings);
}

void ContactListenerImpl::OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair)
{
    ContactListener::OnContactRemoved(inSubShapePair);
}
