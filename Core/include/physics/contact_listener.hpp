#pragma once

#include "core.hpp"
#include "components/collider.hpp"
#include "jolt/Jolt.h"
#include "Jolt/Physics/Collision/ContactListener.h"

BEGIN_XNOR_CORE

class ContactListenerImpl : public JPH::ContactListener
{
public:
    void ProcessEvents();
    
private:
    JPH::ValidateResult OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult) override;
    void OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override;
    void OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override;
    void OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override;

    struct EventInfo
    {
        Collider* self = nullptr;
        Collider* other = nullptr;
        CollisionData data{};

        constexpr EventInfo(Collider* s, Collider* o, const CollisionData& d)
            : self(s), other(o), data(d)
        {}
    };

    enum CollisionEvent
    {
        TriggerEnter,
        TriggerStay,
        TriggerExit,
        CollisionEnter,
        CollisionStay,
        CollisionExit,

        EventCount
    };
    
    std::vector<std::vector<EventInfo>> m_Events = std::vector<std::vector<EventInfo>>(EventCount);
};

END_XNOR_CORE
