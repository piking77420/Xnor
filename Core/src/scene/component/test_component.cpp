#include "scene/component/test_component.hpp"

#include "input/time.hpp"
#include "scene/entity.hpp"
#include "utils/factory.hpp"

using namespace XnorCore;

REFLECTABLE_IMPL_CPP(TestComponent)

void TestComponent::Begin()
{
    m_BasePosition = entity->transform.GetPosition();

    m_Light = entity->GetComponent<Light>();

    m_Color = static_cast<ColorHsva>(ColorRgb::Red());
}

void TestComponent::Update()
{
    if (!m_Rotate || m_Light == nullptr)
        return;

    m_CurrentAngle += m_RotationSpeed * Time::GetDeltaTime();
    entity->transform.SetPosition().x = (std::cos(m_CurrentAngle) - std::sin(m_CurrentAngle)) * m_Radius;
    entity->transform.SetPosition().z = (std::cos(m_CurrentAngle) + std::sin(m_CurrentAngle)) * m_Radius;

    m_Color.h++;

    m_Light->color = static_cast<Colorf>(m_Color);
}
