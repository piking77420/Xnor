#pragma once

#include "core.hpp"
#include "scene/component.hpp"
#include "physics/components/collider.hpp"

BEGIN_XNOR_CORE

class SphereCollider : public Collider
{
    REFLECTABLE_IMPL(SphereCollider)

public:
    XNOR_ENGINE SphereCollider() = default; 
    XNOR_ENGINE ~SphereCollider() override = default;

    XNOR_ENGINE void Begin() override;
    XNOR_ENGINE void Update() override;
    
    DEFAULT_COPY_MOVE_OPERATIONS(SphereCollider)
    
private:
    float_t m_Radius = 1.f;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::SphereCollider, bases<XnorCore::Collider>),
    field(m_Radius)
)
