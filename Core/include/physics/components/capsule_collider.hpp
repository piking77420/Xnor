#pragma once

#include "core.hpp"
#include "scene/component.hpp"
#include "physics/components/collider.hpp"

BEGIN_XNOR_CORE

class CapsuleCollider : public Collider
{
    REFLECTABLE_IMPL(CapsuleCollider)

public:
    XNOR_ENGINE CapsuleCollider() = default; 
    XNOR_ENGINE ~CapsuleCollider() override = default;

    XNOR_ENGINE void Begin() override;
    XNOR_ENGINE void Update() override;
    
    DEFAULT_COPY_MOVE_OPERATIONS(CapsuleCollider)
    
private:
    float_t m_Height = 1.f;
    float_t m_Radius = 1.f;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::CapsuleCollider, bases<XnorCore::Collider>),
    field(m_Height),
    field(m_Radius)
)
