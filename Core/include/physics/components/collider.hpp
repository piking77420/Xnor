#pragma once

#include "core.hpp"
#include "physics/data/collision_data.hpp"
#include "scene/component.hpp"
#include "utils/event.hpp"

BEGIN_XNOR_CORE

class Collider : public Component
{
    REFLECTABLE_IMPL(Collider)

public:
    enum ColliderConstraints : size_t
    {
        ConstraintNone = 0 << 0,
        ConstraintPosition = 1 << 0,
        ConstraintRotation = 1 << 1,
    };

    XNOR_ENGINE Collider(); 
    XNOR_ENGINE ~Collider() override;
    
    DEFAULT_COPY_MOVE_OPERATIONS(Collider)

    XNOR_ENGINE void Begin() override;
    XNOR_ENGINE void Update() override;

    [[nodiscard]]
    XNOR_ENGINE bool_t IsTrigger() const;

    Event<Collider*, const CollisionData&> onTriggerEnter;
    Event<Collider*, const CollisionData&> onTriggerStay;
    Event<Collider*> onTriggerExit;
    Event<Collider*, const CollisionData&> onCollisionEnter;
    Event<Collider*, const CollisionData&> onCollisionStay;
    Event<Collider*> onCollisionExit;

    ColliderConstraints constraints = ConstraintNone;

protected:
    void AddDebugEvents();
    
    uint32_t m_BodyId = std::numeric_limits<uint32_t>::max();

    bool_t m_IsStatic = false;
    bool_t m_IsTrigger = false;
    bool_t m_IsActive = false;
    float_t m_Force;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Collider, bases<XnorCore::Component>),
    field(constraints, XnorCore::Reflection::EnumFlags()),
    field(m_IsStatic),
    field(m_IsTrigger),
    field(m_IsActive, XnorCore::Reflection::ReadOnly()),
    field(m_Force, XnorCore::Reflection::NotSerializable())
)
