#include "physics/components/collider.hpp"

#include "input/input.hpp"
#include "physics/physics_world.hpp"
#include "scene/entity.hpp"
#include "serialization/serializer.hpp"

using namespace XnorCore;

Collider::Collider()
{
    // AddDebugEvents();
}

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
    m_IsActive = PhysicsWorld::IsBodyActive(m_BodyId);

    if (m_IsActive)
    {
        if (!(constraints & ConstraintPosition))
            entity->transform.SetPosition(PhysicsWorld::GetBodyPosition(m_BodyId));
        else
            PhysicsWorld::SetPosition(m_BodyId, entity->transform.GetPosition());
            
        if (!(constraints & ConstraintRotation))
            entity->transform.SetRotationEulerAngle(Quaternion::ToEuler(PhysicsWorld::GetBodyRotation(m_BodyId)));
        else
            PhysicsWorld::SetRotation(m_BodyId, entity->transform.GetRotation());
    }
}

bool_t Collider::IsTrigger() const
{
    return m_IsTrigger;
}

void Collider::AddDebugEvents()
{
    onTriggerEnter += [&](const Collider* const other, const CollisionData& data) -> void
    {
        Logger::LogDebug("OnTriggerEnter between {} and {} ; Normal : {} ; Pen depth : {}", entity->name, other->entity->name, data.normal, data.penetrationDepth);
    };
    
    onTriggerStay += [&](const Collider* const other, const CollisionData& data) -> void
    {
        Logger::LogDebug("OnTriggerStay between {} and {} ; Normal : {} ; Pen depth : {}", entity->name, other->entity->name, data.normal, data.penetrationDepth);
    };

    onTriggerExit += [&](const Collider* const other) -> void
    {
        Logger::LogDebug("OnTriggerExit between {} and {}", entity->name, other->entity->name);
    };

    onCollisionEnter += [&](const Collider* const other, const CollisionData& data) -> void
    {
        Logger::LogDebug("OnCollisionEnter between {} and {} ; Normal : {} ; Pen depth : {}", entity->name, other->entity->name, data.normal, data.penetrationDepth);
    };

    onCollisionStay += [&](const Collider* const other, const CollisionData& data) -> void
    {
        Logger::LogDebug("OnCollisionStay between {} and {} ; Normal : {} ; Pen depth : {}", entity->name, other->entity->name, data.normal, data.penetrationDepth);
    };

    onCollisionExit += [&](const Collider* const other) -> void
    {
        Logger::LogDebug("OnCollisionExit between {} and {}", entity->name, other->entity->name);
    };
}
