#pragma once

#include "core.hpp"
#include "scene/component.hpp"
#include "physics/components/collider.hpp"

BEGIN_XNOR_CORE

class BoxCollider : public Collider
{
    REFLECTABLE_IMPL_MINIMAL(BoxCollider)

public:
    XNOR_ENGINE BoxCollider() = default; 
    XNOR_ENGINE ~BoxCollider() override = default;

    XNOR_ENGINE void Begin() override;
    XNOR_ENGINE void Update() override;
    
    DEFAULT_COPY_MOVE_OPERATIONS(BoxCollider)

private:
    Vector3 m_Center = Vector3::Zero();
    Quaternion m_Rotation = Quaternion::Identity();
    Vector3 m_Size = Vector3(1.f);
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::BoxCollider, bases<XnorCore::Collider>),
    field(m_Center),
    field(m_Size)
)
