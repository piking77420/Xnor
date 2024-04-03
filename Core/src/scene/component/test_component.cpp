#include "scene/component/test_component.hpp"

#include "input/time.hpp"
#include "rendering/light/light.hpp"
#include "scene/entity.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void TestComponent::Begin()
{
    m_BasePosition = GetEntity()->transform.GetPosition();

    m_Light = GetEntity()->GetComponent<Light>();

    m_Color = static_cast<ColorHsva>(ColorRgb::Red());
}

void TestComponent::Update()
{
    if (!m_Rotate || m_Light == nullptr)
        return;

    m_CurrentAngle += m_RotationSpeed * Time::GetDeltaTime();
    GetEntity()->transform.SetPositionX((std::cos(m_CurrentAngle) - std::sin(m_CurrentAngle)) * m_Radius);
    GetEntity()->transform.SetPositionZ((std::cos(m_CurrentAngle) + std::sin(m_CurrentAngle)) * m_Radius);

    m_Color.h += 0.01f;

    m_Light->color = static_cast<Colorf>(m_Color);
}
