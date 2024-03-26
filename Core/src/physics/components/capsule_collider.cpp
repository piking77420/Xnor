#include "physics/components/capsule_collider.hpp"

#include "physics/physics_world.hpp"
#include "physics/components/collider.hpp"
#include "scene/entity.hpp"

using namespace XnorCore;

void CapsuleCollider::Begin()
{
    const Transform& t = entity->transform;

    m_BodyId = PhysicsWorld::CreateCapsule(this, t.GetPosition(), t.GetRotation(), t.GetScale(), m_IsTrigger, m_IsStatic, m_Height, m_Radius);
}

void CapsuleCollider::Update()
{
    Collider::Update();
}
