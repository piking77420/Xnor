#include "scene/component/test_component.hpp"

#include "input/time.hpp"
#include "scene/entity.hpp"

using namespace XnorCore;

REFLECTABLE_IMPL_CPP(TestComponent)

void TestComponent::Begin()
{
    m_BasePosition = entity->transform.GetPosition();

    light = entity->GetComponent<Light>();

    color = static_cast<ColorHsv>(ColorRgb::Red());
}

void TestComponent::Update()
{
    if (!m_Rotate || light == nullptr)
        return;

    m_CurrentAngle += m_RotationSpeed * Time::GetDeltaTime();
    entity->transform.SetPosition().x = (std::cos(m_CurrentAngle) - std::sin(m_CurrentAngle)) * m_Radius;
    entity->transform.SetPosition().z = (std::cos(m_CurrentAngle) + std::sin(m_CurrentAngle)) * m_Radius;

    color.h++;

    light->color = static_cast<Colorf>(color);
}
