#include "physics/components/box_collider.hpp"

#include "physics/physics_world.hpp"
#include "physics/components/collider.hpp"
#include "scene/entity.hpp"
#include "serialization/serializer.hpp"

using namespace XnorCore;

REFLECTABLE_IMPL_CPP(BoxCollider)

void BoxCollider::Begin()
{
    m_BodyId = PhysicsWorld::CreateBox(entity->transform.GetPosition(), entity->transform.GetRotation(), entity->transform.GetScale());
}

void BoxCollider::Update()
{
    Collider::Update();
}
