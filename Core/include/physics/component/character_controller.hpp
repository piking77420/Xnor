#pragma once

#include "core.hpp"
#include "physics/component/collider.hpp"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Character/Character.h>

BEGIN_XNOR_CORE

class CharacterController : public Collider
{
    REFLECTABLE_IMPL(CharacterController)
    
public:
    XNOR_ENGINE CharacterController() = default;
    
    XNOR_ENGINE ~CharacterController() override;

    DEFAULT_COPY_MOVE_OPERATIONS(CharacterController)

    void Begin() override;

    [[nodiscard]]
    XNOR_ENGINE Vector3 GetVelocity() const;
    XNOR_ENGINE void SetVelocity(const Vector3& velocity) const;

private:
    JPH::Character* m_Character = nullptr;
    JPH::Ref<JPH::Shape> m_StandingShape;

    float_t m_Friction = 0.2f;
    float_t m_GravityFactor = 1.f;
    float_t m_Mass = 80.f;
    float_t m_MaxSlopeAngle = Calc::Deg2Rad * 45.f;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::CharacterController, bases<XnorCore::Collider>),
    field(m_Friction),
    field(m_GravityFactor, XnorCore::Reflection::Range(0.f, 1.f)),
    field(m_Mass),
    field(m_MaxSlopeAngle, XnorCore::Reflection::AsAngle())
)
