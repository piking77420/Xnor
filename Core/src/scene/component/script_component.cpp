#include "scene/component/script_component.hpp"

using namespace XnorCore;

ScriptComponent::~ScriptComponent()
{
    m_ManagedObject.Destroy();
}

void ScriptComponent::Initialize(const Coral::ManagedObject& managedObject)
{
    m_ManagedObject = managedObject;
}

void ScriptComponent::Begin()
{
    m_ManagedObject.InvokeMethod("Begin");
}

void ScriptComponent::Update()
{
    m_ManagedObject.InvokeMethod("Update");
}

Coral::ManagedObject& ScriptComponent::GetManagedObject()
{
    return m_ManagedObject;
}
