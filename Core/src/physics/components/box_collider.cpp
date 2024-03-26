#include "physics/components/box_collider.hpp"

#include "physics/physics_world.hpp"
#include "physics/components/collider.hpp"
#include "scene/entity.hpp"
#include "serialization/serializer.hpp"

using namespace XnorCore;

void BoxCollider::Begin()
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

    m_BodyId = PhysicsWorld::CreateBox(info);
}

void BoxCollider::Update()
{
    Collider::Update();
}
