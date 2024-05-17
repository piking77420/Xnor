#include "physics/component/character_controller.hpp"

#include "transform.hpp"
#include "physics/physics_world.hpp"

using namespace XnorCore;

CharacterController::~CharacterController()
{
    if (m_Character)
        m_Character->RemoveFromPhysicsSystem();
    delete m_Character;
}

void CharacterController::Begin()
{
    Collider::Begin();

    const Transform& transform = GetTransform();

    JPH::CharacterSettings settings;
    settings.mFriction = m_Friction;
    settings.mGravityFactor = m_GravityFactor;
    settings.mMass = m_Mass;
    settings.mMaxSlopeAngle = m_MaxSlopeAngle;
    settings.mShape = m_StandingShape;

    m_Character = PhysicsWorld::CreateCharacter(settings, transform.GetPosition(), transform.GetRotation());
}

Vector3 CharacterController::GetVelocity() const
{
    return PhysicsWorld::FromJph(m_Character->GetLinearVelocity());
}

void CharacterController::SetVelocity(const Vector3& velocity) const
{
    m_Character->SetLinearVelocity(PhysicsWorld::ToJph(velocity));
}
