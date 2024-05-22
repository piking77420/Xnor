#include "physics/component/sphere_collider.hpp"

#include "physics/physics_world.hpp"
#include "physics/component/collider.hpp"
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

    m_BodyId = PhysicsWorld::CreateSphere(info, radius);
    
    SetFriction(m_Friction);
    PrePhysics();
}

void SphereCollider::Update()
{
    Collider::Update();
}

void SphereCollider::SetRadius(float_t newRadius)
{
    PhysicsWorld::DestroyBody(m_BodyId);
    const Transform& t = entity->transform;

    const PhysicsWorld::BodyCreationInfo info = {
        .collider = this,
        .position = t.GetPosition(),
        .rotation = t.GetRotation(),
        .scaling = t.GetScale(),
        .isTrigger = m_IsTrigger,
        .isStatic = m_IsStatic
    };
    radius = newRadius;
    m_BodyId = PhysicsWorld::CreateSphere(info, radius);

}
