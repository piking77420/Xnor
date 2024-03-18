#include "scene/component/script_component.hpp"

#include "utils/factory.hpp"

using namespace XnorCore;

REFLECTABLE_IMPL_CPP(ScriptComponent)

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
    Component::Begin();

    m_ManagedObject.InvokeMethod("Begin");
}

void ScriptComponent::Update()
{
    Component::Update();

    m_ManagedObject.InvokeMethod("Update");
}
