#include "scene/component/test_component.hpp"

#include "input/time.hpp"
#include "scene/entity.hpp"

using namespace XnorCore;

REFLECTABLE_IMPL_CPP(TestComponent)

void TestComponent::Begin()
{
    m_BasePosition = entity->transform.position;
}

void TestComponent::Update()
{
    if (!m_Rotate)
        return;

    m_CurrentAngle += m_RotationSpeed * Time::GetDeltaTime();
    entity->transform.position.x = (std::cos(m_CurrentAngle) - std::sin(m_CurrentAngle)) * m_Radius;
    entity->transform.position.z = (std::cos(m_CurrentAngle) + std::sin(m_CurrentAngle)) * m_Radius;
    entity->transform.changed = true;
}
