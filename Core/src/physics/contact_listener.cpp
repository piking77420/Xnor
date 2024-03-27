#include "physics/contact_listener.hpp"

#include "Jolt/Physics/Body/Body.h"
#include "physics/physics_world.hpp"
#include "physics/data/collision_data.hpp"
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
    const JPH::Body& inBody1,
    const JPH::Body& inBody2,
    const JPH::ContactManifold& inManifold,
    [[maybe_unused]] JPH::ContactSettings& ioSettings
)
{
    const uint32_t bodyId1 = inBody1.GetID().GetIndexAndSequenceNumber();
    const uint32_t bodyId2 = inBody2.GetID().GetIndexAndSequenceNumber();
    Collider* const c1 = PhysicsWorld::GetColliderFromId(bodyId1);
    Collider* const c2 = PhysicsWorld::GetColliderFromId(bodyId2);

    const CollisionData data = {
        .penetrationDepth = inManifold.mPenetrationDepth,
        .normal = Vector3(inManifold.mWorldSpaceNormal.GetX(), inManifold.mWorldSpaceNormal.GetY(), inManifold.mWorldSpaceNormal.GetZ())
    };

    if (inBody1.IsSensor() && !inBody2.IsSensor())
    {
        // Body 1 is the trigger, so body 2 entered in it
        c1->onTriggerEnter.Invoke(c1, c2, data);
    }
    else if (!inBody1.IsSensor() && inBody2.IsSensor())
    {
        // Body 2 is the trigger, so body 1 entered in it
        c2->onTriggerEnter.Invoke(c2, c1, data);
    }
    else if (!inBody1.IsSensor() && !inBody2.IsSensor())
    {
        // Both body aren't triggers, so it's a normal collision
        // We call the events of both of them unlike the triggers
        c1->onCollisionEnter.Invoke(c1, c2, data);
        c2->onCollisionEnter.Invoke(c2, c1, data);
    }
    else
    {
        // TODO handle trigger + trigger collision
    }
}

void ContactListenerImpl::OnContactPersisted(
    const JPH::Body& inBody1,
    const JPH::Body& inBody2,
    const JPH::ContactManifold& inManifold,
    [[maybe_unused]] JPH::ContactSettings& ioSettings
)
{
    const uint32_t bodyId1 = inBody1.GetID().GetIndexAndSequenceNumber();
    const uint32_t bodyId2 = inBody2.GetID().GetIndexAndSequenceNumber();
    Collider* const c1 = PhysicsWorld::GetColliderFromId(bodyId1);
    Collider* const c2 = PhysicsWorld::GetColliderFromId(bodyId2);

    const CollisionData data = {
        .penetrationDepth = inManifold.mPenetrationDepth,
        .normal = Vector3(inManifold.mWorldSpaceNormal.GetX(), inManifold.mWorldSpaceNormal.GetY(), inManifold.mWorldSpaceNormal.GetZ())
    };
    
    if (inBody1.IsSensor() && !inBody2.IsSensor())
    {
        // Body 1 is the trigger, so body 2 entered in it
        c1->onTriggerStay.Invoke(c1, c2, data);
    }
    else if (!inBody1.IsSensor() && inBody2.IsSensor())
    {
        // Body 2 is the trigger, so body 1 entered in it
        c2->onTriggerStay.Invoke(c2, c1, data);
    }
    else if (!inBody1.IsSensor() && !inBody2.IsSensor())
    {
        // Both body aren't triggers, so it's a normal collision
        // We call the events of both of them unlike the triggers
        c1->onCollisionStay.Invoke(c1, c2, data);
        c2->onCollisionStay.Invoke(c2, c1, data);
    }
    else
    {
        // TODO handle trigger + trigger collision
    }
}

void ContactListenerImpl::OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair)
{
    const uint32_t bodyId1 = inSubShapePair.GetBody1ID().GetIndexAndSequenceNumber();
    const uint32_t bodyId2 = inSubShapePair.GetBody2ID().GetIndexAndSequenceNumber();
    Collider* const c1 = PhysicsWorld::GetColliderFromId(bodyId1);
    Collider* const c2 = PhysicsWorld::GetColliderFromId(bodyId2);

    if (c1 == nullptr || c2 == nullptr)
        return;
    
    if (c1->IsTrigger() && !c2->IsTrigger())
    {
        // Body 1 is the trigger, so body 2 entered in it
        c1->onTriggerExit.Invoke(c1, c2);
    }
    else if (!c1->IsTrigger() && c2->IsTrigger())
    {
        // Body 2 is the trigger, so body 1 entered in it
        c2->onTriggerExit.Invoke(c2, c1);
    }
}
