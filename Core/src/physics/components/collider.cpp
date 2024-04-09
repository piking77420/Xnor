#include "physics/components/collider.hpp"

#include "input/input.hpp"
#include "physics/physics_world.hpp"
#include "scene/entity.hpp"
#include "serialization/serializer.hpp"

using namespace XnorCore;

Collider::Collider()
{
    AddDebugEvents();
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
}

void Collider::PrePhysics()
{
    if (m_IsActive)
    {
        PhysicsWorld::SetPosition(m_BodyId, entity->transform.GetPosition());
        PhysicsWorld::SetRotation(m_BodyId, entity->transform.GetRotation());
    }
}

void Collider::PostPhysics()
{
    m_IsActive = PhysicsWorld::IsBodyActive(m_BodyId);

    if (!m_IsActive)
        return;
        
    if (!(constraints & ConstraintPosition))
        entity->transform.SetPosition(PhysicsWorld::GetBodyPosition(m_BodyId));

    if (!(constraints & ConstraintRotation))
        entity->transform.SetRotationEulerAngle(Quaternion::ToEuler(PhysicsWorld::GetBodyRotation(m_BodyId)));
}

bool_t Collider::IsTrigger() const
{
    return m_IsTrigger;
}

void Collider::AddForce(const Vector3& force) const
{
    PhysicsWorld::AddForce(m_BodyId, force);
}

void Collider::AddDebugEvents()
{
    std::vector<std::function<void(Collider*, Collider*, const CollisionData&)>> v;
    v.emplace_back(Test);
    std::function<void(Collider*, Collider*, const CollisionData&)> lambda = [](const Collider* const self, const Collider* const other, const CollisionData& data) -> void
    {
        Logger::LogDebug("OnTriggerEnter between {} and {} ; Normal : {} ; Pen depth : {}", self->entity->name, other->entity->name, data.normal, data.penetrationDepth);
    };
    v.emplace_back(lambda);
    v.clear();

    List<std::function<void()>> l1(2);
    l1.Add([]{});
    l1.Add([]{});
    l1.Clear();

    List<std::function<void(Collider*, Collider*, const CollisionData&)>> l;
    l.Add(Test);
    l.Add(lambda);
    using T = typename decltype(l)::Type;
    l.RemoveAt(l.GetSize() - 1);
    l.Clear();

    onTriggerEnter += [](const Collider* const self, const Collider* const other, const CollisionData& data) -> void
    {
        Logger::LogDebug("OnTriggerEnter between {} and {} ; Normal : {} ; Pen depth : {}", self->entity->name, other->entity->name, data.normal, data.penetrationDepth);
    };

    onTriggerEnter += Test;

    onTriggerEnter.Clear();
    
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
