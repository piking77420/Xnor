#include "scene/component/script_component.hpp"

#include "scene/entity.hpp"

using namespace XnorCore;

void ScriptComponent::Initialize(const Coral::ManagedObject& managedObject)
{
    m_ManagedObject = managedObject;
}

void ScriptComponent::Destroy()
{
    m_ManagedObject.Destroy();
}

void ScriptComponent::Begin()
{
    Collider* collider = nullptr;
    if (entity->TryGetComponent(&collider))
    {
        collider->onTriggerEnter += [this](Collider* self, Collider* other, const CollisionData& data) { OnTriggerEnter(self, other, data); };
        collider->onTriggerStay += [this](Collider* self, Collider* other, const CollisionData& data) { OnTriggerStay(self, other, data); };
        collider->onTriggerExit += [this](Collider* self, Collider* other) { OnTriggerExit(self, other); };
        collider->onCollisionEnter += [this](Collider* self, Collider* other, const CollisionData& data) { OnCollisionEnter(self, other, data); };
        collider->onCollisionStay += [this](Collider* self, Collider* other, const CollisionData& data) { OnCollisionStay(self, other, data); };
        collider->onCollisionExit += [this](Collider* self, Collider* other) { OnCollisionExit(self, other); };
    }
    
    m_ManagedObject.InvokeMethod("Begin");
}

void ScriptComponent::Update()
{
    m_ManagedObject.InvokeMethod("Update");
}

void ScriptComponent::OnTriggerEnter(Collider* self, Collider* other, const CollisionData& data)
{
    m_ManagedObject.InvokeMethod(
        "OnTriggerEnter",
        std::forward<Collider*>(self),
        std::forward<Collider*>(other),
        std::forward<CollisionData*>(&const_cast<CollisionData&>(data))
    );
}

void ScriptComponent::OnTriggerStay(Collider* self, Collider* other, const CollisionData& data)
{
    m_ManagedObject.InvokeMethod("OnTriggerStay", std::forward<Collider*>(self), std::forward<Collider*>(other), std::forward<const CollisionData&>(data));
}

void ScriptComponent::OnTriggerExit(Collider* self, Collider* other)
{
    m_ManagedObject.InvokeMethod("OnTriggerExit", std::forward<Collider*>(self), std::forward<Collider*>(other));
}

void ScriptComponent::OnCollisionEnter(Collider* self, Collider* other, const CollisionData& data)
{
    m_ManagedObject.InvokeMethod("OnCollisionEnter", std::forward<Collider*>(self), std::forward<Collider*>(other), std::forward<const CollisionData&>(data));
}

void ScriptComponent::OnCollisionStay(Collider* self, Collider* other, const CollisionData& data)
{
    m_ManagedObject.InvokeMethod("OnCollisionStay", std::forward<Collider*>(self), std::forward<Collider*>(other), std::forward<const CollisionData&>(data));
}

void ScriptComponent::OnCollisionExit(Collider* self, Collider* other)
{
    m_ManagedObject.InvokeMethod("OnCollisionExit", std::forward<Collider*>(self), std::forward<Collider*>(other));
}

Coral::ManagedObject& ScriptComponent::GetManagedObject()
{
    return m_ManagedObject;
}

const Coral::ManagedObject& ScriptComponent::GetManagedObject() const
{
    return m_ManagedObject;
}
