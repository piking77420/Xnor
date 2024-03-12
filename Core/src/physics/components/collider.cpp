#include "physics/components/collider.hpp"

#include "physics/physics_world.hpp"
#include "scene/entity.hpp"
#include "serialization/serializer.hpp"

using namespace XnorCore;

REFLECTABLE_IMPL_CPP(Collider)

Collider::~Collider()
{
    if (!JPH::BodyID(m_BodyId).IsInvalid())
        PhysicsWorld::DestroyBody(m_BodyId);
}

void Collider::Begin()
{
}

void Collider::Update()
{
    if (PhysicsWorld::IsBodyActive(m_BodyId))
    {
        if (!(constraints & ConstraintPosition))
            entity->transform.SetPosition() = PhysicsWorld::GetBodyPosition(m_BodyId);
        else
            PhysicsWorld::SetPosition(m_BodyId, entity->transform.GetPosition());
            
        if (!(constraints & ConstraintRotation))
            entity->transform.SetRotationEulerAngle() = Quaternion::ToEuler(PhysicsWorld::GetBodyRotation(m_BodyId));
        else
            PhysicsWorld::SetRotation(m_BodyId, entity->transform.GetRotation());
    }
}
