#include "physics/components/sphere_collider.hpp"

#include "physics/physics_world.hpp"
#include "physics/components/collider.hpp"
#include "scene/entity.hpp"
#include "serialization/serializer.hpp"

using namespace XnorCore;


void SphereCollider::Begin()
{
    const Transform& t = entity->transform;

    const PhysicsWorld::BodyCreationInfo info = {
        .collider = this,
        .position = t.GetPosition(),
        .rotation = t.GetRotation(),
        .scaling = t.GetScale(),
        .isTrigger = m_IsTrigger,
        .isStatic = m_IsStatic
    };

    m_BodyId = PhysicsWorld::CreateSphere(info, m_Radius);
}

void SphereCollider::Update()
{
    Collider::Update();
}
