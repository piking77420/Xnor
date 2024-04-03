#pragma once

#include "core.hpp"
#include "scene/component.hpp"
#include "physics/components/collider.hpp"

/// @file capsule_collider.hpp
/// @brief Defines the XnorCore::CapsuleCollider class

BEGIN_XNOR_CORE

/// @brief Capsule collider
class CapsuleCollider : public Collider
{
    REFLECTABLE_IMPL(CapsuleCollider)

public:
    XNOR_ENGINE CapsuleCollider() = default; 
    XNOR_ENGINE ~CapsuleCollider() override = default;
    
    DEFAULT_COPY_MOVE_OPERATIONS(CapsuleCollider)

    /// @brief Begin function
    XNOR_ENGINE void Begin() override;
    /// @brief Update function
    XNOR_ENGINE void Update() override;

private:
    float_t m_Height = 1.f;
    float_t m_Radius = 1.f;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::CapsuleCollider, bases<XnorCore::Collider>),
    field(m_Height),
    field(m_Radius)
)
