#include "physics/contact_listener.hpp"

#include "Jolt/Physics/Body/Body.h"
#include "physics/physics_world.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;


JPH::ValidateResult ContactListenerImpl::OnContactValidate(
    [[maybe_unused]] const JPH::Body& inBody1,
    [[maybe_unused]] const JPH::Body& inBody2,
    [[maybe_unused]] const JPH::RVec3Arg inBaseOffset,
    [[maybe_unused]] const JPH::CollideShapeResult& inCollisionResult
)
{
    // Logger::LogInfo("Contact between {} and {}", inBody1.GetID().GetIndexAndSequenceNumber(), inBody2.GetID().GetIndexAndSequenceNumber());

    return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
}

void ContactListenerImpl::OnContactAdded(
    [[maybe_unused]] const JPH::Body& inBody1,
    [[maybe_unused]] const JPH::Body& inBody2,
    [[maybe_unused]] const JPH::ContactManifold& inManifold,
    [[maybe_unused]] JPH::ContactSettings& ioSettings
)
{
    const uint32_t bodyId1 = inBody1.GetID().GetIndexAndSequenceNumber();
    const uint32_t bodyId2 = inBody2.GetID().GetIndexAndSequenceNumber();
    Collider* const c1 = PhysicsWorld::GetColliderFromId(bodyId1);
    Collider* const c2 = PhysicsWorld::GetColliderFromId(bodyId2);

    if (inBody1.IsSensor() && !inBody2.IsSensor())
    {
        // Body 1 is the trigger, so body 2 entered in it
        c1->OnTriggerEnter(c2);
    }
    else if (!inBody1.IsSensor() && inBody2.IsSensor())
    {
        // Body 2 is the trigger, so body 1 entered in it
        c2->OnTriggerEnter(c1);
    }
}

void ContactListenerImpl::OnContactPersisted(
    [[maybe_unused]] const JPH::Body& inBody1,
    [[maybe_unused]] const JPH::Body& inBody2,
    [[maybe_unused]] const JPH::ContactManifold& inManifold,
    [[maybe_unused]] JPH::ContactSettings& ioSettings
)
{
    // Logger::LogInfo("Contact persisted between {} and {}", inBody1.GetID().GetIndexAndSequenceNumber(), inBody2.GetID().GetIndexAndSequenceNumber());
}

void ContactListenerImpl::OnContactRemoved([[maybe_unused]] const JPH::SubShapeIDPair& inSubShapePair)
{
    // Logger::LogInfo("Contact removed {} and {}", inSubShapePair.GetBody1ID().GetIndexAndSequenceNumber(), inSubShapePair.GetBody1ID().GetIndexAndSequenceNumber());
}
