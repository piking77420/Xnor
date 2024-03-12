#pragma once

#include "core.hpp"
#include "scene/component.hpp"
#include "physics/components/collider.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE SphereCollider : public Collider
{
    REFLECTABLE_IMPL_MINIMAL(SphereCollider)

public:
    SphereCollider() = default; 
    ~SphereCollider() override = default;

    void Begin() override;
    void Update() override;
    
    DEFAULT_COPY_MOVE_OPERATIONS(SphereCollider)
    
private:
    float_t m_Radius = 1.f;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::SphereCollider, bases<XnorCore::Collider>),
    field(m_Radius)
)
