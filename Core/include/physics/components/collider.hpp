#pragma once

#include "core.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE Collider : public Component
{
    REFLECTABLE_IMPL_MINIMAL(Collider)

public:
    enum ColliderConstraints : size_t
    {
        ConstraintNone = 0 << 0,
        ConstraintPosition = 1 << 0,
        ConstraintRotation = 1 << 1,
    };
    
    Collider() = default; 
    ~Collider() override;

    void Begin() override;
    void Update() override;

    DEFAULT_COPY_MOVE_OPERATIONS(Collider)

    ColliderConstraints constraints = ConstraintNone;

protected:
    uint32_t m_BodyId = std::numeric_limits<uint32_t>::max();
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Collider, bases<XnorCore::Component>),
    field(constraints, XnorCore::Reflection::EnumFlags())
)
