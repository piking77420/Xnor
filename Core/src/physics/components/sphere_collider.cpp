#include "physics/components/sphere_collider.hpp"

#include "physics/physics_world.hpp"
#include "physics/components/collider.hpp"
#include "scene/entity.hpp"
#include "serialization/serializer.hpp"

using namespace XnorCore;

REFLECTABLE_IMPL_CPP(SphereCollider)

void SphereCollider::Begin()
{
    m_BodyId = PhysicsWorld::CreateSphere(this, entity->transform.GetPosition(), m_Radius, m_IsTrigger, m_IsStatic);
}

void SphereCollider::Update()
{
    Collider::Update();
}
