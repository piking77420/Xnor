#pragma once

#include "core.hpp"
#include "scene/component.hpp"
#include "utils/event.hpp"

BEGIN_XNOR_CORE

class Collider : public Component
{
    REFLECTABLE_IMPL_MINIMAL(Collider)

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

    XNOR_ENGINE void OnTriggerEnter(Collider* other);

    Event<Collider*> onTriggerEnter;
    ColliderConstraints constraints = ConstraintNone;

protected:
    uint32_t m_BodyId = std::numeric_limits<uint32_t>::max();
    bool_t m_IsTrigger = false;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Collider, bases<XnorCore::Component>),
    field(constraints, XnorCore::Reflection::EnumFlags()),
    field(m_IsTrigger)
)
