#include "physics/component/collider.hpp"

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

void Collider::Awake()
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

    SetFriction(m_Friction);
}

void Collider::Begin()
{
}

void Collider::Update()
{
}

void Collider::PrePhysics()
{
    if (m_IsActive)
    {
        PhysicsWorld::SetPosition(m_BodyId, entity->transform.GetPosition());
        PhysicsWorld::SetRotation(m_BodyId, entity->transform.GetRotation().Normalized());
    }
}

void Collider::PostPhysics()
{
    m_IsActive = PhysicsWorld::IsBodyActive(m_BodyId);

    if (!m_IsActive)
        return;

    

    if (!(constraints & ConstraintPosition))
    {
        entity->transform.SetPosition(PhysicsWorld::GetBodyPosition(m_BodyId));
    }

    if (!(constraints & ConstraintRotation))
    {
        entity->transform.SetRotationEulerAngle(Quaternion::ToEuler(PhysicsWorld::GetBodyRotation(m_BodyId)));
    }


}

bool_t Collider::IsTrigger() const
{
    return m_IsTrigger;
}

void Collider::AddForce(const Vector3& force) const
{
    PhysicsWorld::AddForce(m_BodyId, force);
}

void Collider::AddImpulse(const Vector3& impulse) const
{
    PhysicsWorld::AddImpulse(m_BodyId, impulse);
}

void Collider::SetFriction(const float_t friction)
{
    m_Friction = friction;
    PhysicsWorld::SetFriction(m_BodyId, friction);
}

float_t Collider::GetFriction() const
{
    return PhysicsWorld::GetFriction(m_BodyId);
}

void Collider::SetLinearVelocity(Vector3 velocity)
{
    PhysicsWorld::SetLinearVelocity(m_BodyId,velocity);
}

Vector3 Collider::GetLinearVelocity() const
{
    return PhysicsWorld::GetLinearVelocity(m_BodyId);
}

void Collider::AddDebugEvents()
{
    onTriggerEnter += [](const Collider* const self, const Collider* const other, const CollisionData& data) -> void
    {
        Logger::LogDebug("OnTriggerEnter between {} and {} ; Normal : {} ; Pen depth : {}", self->entity->name, other->entity->name, data.normal, data.penetrationDepth);
    };

    onTriggerStay += [](const Collider* const self, const Collider* const other, const CollisionData& data) -> void
    {
        Logger::LogDebug("OnTriggerStay between {} and {} ; Normal : {} ; Pen depth : {}", self->entity->name, other->entity->name, data.normal, data.penetrationDepth);
    };

    onTriggerExit += [](const Collider* const self, const Collider* const other) -> void
    {
        Logger::LogDebug("OnTriggerExit between {} and {}", self->entity->name, other->entity->name);
    };

    onCollisionEnter += [](const Collider* const self, const Collider* const other, const CollisionData& data) -> void
    {
        Logger::LogDebug("OnCollisionEnter between {} and {} ; Normal : {} ; Pen depth : {}", self->entity->name, other->entity->name, data.normal, data.penetrationDepth);
    };

    onCollisionStay += [](const Collider* const self, const Collider* const other, const CollisionData& data) -> void
    {
        Logger::LogDebug("OnCollisionStay between {} and {} ; Normal : {} ; Pen depth : {}", self->entity->name, other->entity->name, data.normal, data.penetrationDepth);
    };

    onCollisionExit += [](const Collider* const self, const Collider* const other) -> void
    {
        Logger::LogDebug("OnCollisionExit between {} and {}", self->entity->name, other->entity->name);
    };
}
