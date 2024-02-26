#include "scene/component/test_component.hpp"

#include "input/time.hpp"
#include "scene/entity.hpp"

using namespace XnorCore;

TestComponent::TestComponent()
{
    CreateTypeInfo();
}

void TestComponent::Begin()
{
    Component::Begin();
    m_BasePosition = entity->transform.position;
}

void TestComponent::Update()
{
    Component::Update();

    if(!m_Rotate)
        return;

    m_CurrentAngle += m_RotationSpeed * Time::GetDeltaTime();
    entity->transform.rotation.x = (std::cos(m_CurrentAngle) - std::sin(m_CurrentAngle)) * m_Radius;
    entity->transform.rotation.z = std::sin(m_CurrentAngle) * m_Radius;
    entity->transform.rotation.z = (std::cos(m_CurrentAngle) + std::sin(m_CurrentAngle)) * m_Radius;
}
