#include "scene/component/script_component.hpp"

#include "csharp/dotnet_assembly.hpp"
#include "csharp/dotnet_constants.hpp"
#include "reflection/xnor_factory.hpp"
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
    InvokeCollisionEvent("OnTriggerEnter", FORWARD(self), FORWARD(other), FORWARD(data));
}

void ScriptComponent::OnTriggerStay(Collider* self, Collider* other, const CollisionData& data)
{
    InvokeCollisionEvent("OnTriggerStay", FORWARD(self), FORWARD(other), FORWARD(data));
}

void ScriptComponent::OnTriggerExit(Collider* self, Collider* other)
{
    InvokeCollisionExitEvent("OnTriggerExit", FORWARD(self), FORWARD(other));
}

void ScriptComponent::OnCollisionEnter(Collider* self, Collider* other, const CollisionData& data)
{
    InvokeCollisionEvent("OnCollisionEnter", FORWARD(self), FORWARD(other), FORWARD(data));
}

void ScriptComponent::OnCollisionStay(Collider* self, Collider* other, const CollisionData& data)
{
    InvokeCollisionEvent("OnCollisionStay", FORWARD(self), FORWARD(other), FORWARD(data));
}

void ScriptComponent::OnCollisionExit(Collider* self, Collider* other)
{
    InvokeCollisionExitEvent("OnCollisionExit", FORWARD(self), FORWARD(other));
}

Coral::ManagedObject& ScriptComponent::GetManagedObject()
{
    return m_ManagedObject;
}

const Coral::ManagedObject& ScriptComponent::GetManagedObject() const
{
    return m_ManagedObject;
}

void ScriptComponent::InvokeCollisionEvent(const std::string& functionName, Collider* self, Collider* other, const CollisionData& data)
{
    const Coral::ManagedAssembly* assembly = DotnetAssembly::xnorCoreAssembly->GetCoralAssembly();
    
    Coral::Type& colliderType = assembly->GetType(Dotnet::XnorCoreNamespace + ".Collider");
    Coral::Type& collisionDataType = assembly->GetType(Dotnet::XnorCoreNamespace + ".CollisionData");

    const std::array managedObjects
    {
        colliderType.CreateInstance(FORWARD(self), false),
        colliderType.CreateInstance(FORWARD(other), false),
        collisionDataType.CreateInstance(&data, false)
    };

    // Manipulate the data to forward them as references to ManagedObject pointers in .NET GC
    // This is done through the InvokeCollisionData struct which serves as the reference
    // We need to do this to trick Coral into thinking we send actual data because the struct type will not be
    // considered a pointer while the actual data we send is the void pointer to the GC handle with the ManagedObject
    std::array<InvokeCollisionData, 3> managedObjectRefs{};

    for (size_t i = 0; i < 3; i++)
        managedObjectRefs.at(i) = { managedObjects.at(i).GetHandle() };
    
    m_ManagedObject.InvokeMethod(functionName, managedObjectRefs[0], managedObjectRefs[1], managedObjectRefs[2]);
}

void ScriptComponent::InvokeCollisionExitEvent(const std::string& functionName, Collider* self, Collider* other)
{
    const Coral::ManagedAssembly* assembly = DotnetAssembly::xnorCoreAssembly->GetCoralAssembly();
    
    Coral::Type& colliderType = assembly->GetType(Dotnet::XnorCoreNamespace + ".Collider");

    const std::array managedObjects
    {
        colliderType.CreateInstance(FORWARD(self), false),
        colliderType.CreateInstance(FORWARD(other), false),
    };

    // Manipulate the data to forward the parameters as references to ManagedObject pointers in the .NET GC
    // This is done through the InvokeCollisionData struct which serves as the reference
    // We need to do this to trick Coral into thinking we send actual data because the struct type will not be
    // considered a pointer while the actual data we send is the void pointer to the GC handle with the ManagedObject
    std::array<InvokeCollisionData, 3> managedObjectRefs{};

    for (size_t i = 0; i < 2; i++)
        managedObjectRefs.at(i) = { managedObjects.at(i).GetHandle() };
    
    m_ManagedObject.InvokeMethod(functionName, managedObjectRefs[0], managedObjectRefs[1]);
}
