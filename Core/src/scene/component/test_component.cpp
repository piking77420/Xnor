#include "scene/component/test_component.hpp"

#include "input/time.hpp"
#include "rendering/light/light.hpp"
#include "resource/resource_manager.hpp"
#include "scene/entity.hpp"
#include "scene/component/skinned_mesh_renderer.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void TestComponent::Begin()
{
    m_BasePosition = GetEntity()->transform.GetPosition();

    m_Light = GetEntity()->GetComponent<Light>();

    SkinnedMeshRenderer* smr = nullptr;
    if (GetEntity()->TryGetComponent<SkinnedMeshRenderer>(&smr))
    {
        smr->StartBlending(ResourceManager::Get<Mesh>("assets/models/Coyote-Walking.fbx")->GetAnimation(0));
    }

    m_Color = ColorHsva::Red();
}

void TestComponent::Update()
{
    if (!m_Rotate || m_Light == nullptr)
        return;

    if (coroutineId == Guid::Empty())
        coroutineId = Coroutine::Start(FlashRoutine());

    m_CurrentAngle += m_RotationSpeed * Time::GetDeltaTime();
    GetEntity()->transform.SetPositionX((std::cos(m_CurrentAngle) - std::sin(m_CurrentAngle)) * m_Radius);
    GetEntity()->transform.SetPositionZ((std::cos(m_CurrentAngle) + std::sin(m_CurrentAngle)) * m_Radius);

    m_Color.h += 0.01f;

    m_Light->color = static_cast<Color>(m_Color);
}

Coroutine TestComponent::FlashRoutine()
{
    using namespace std::chrono_literals;

    m_Light->intensity = 1000.f;

    while (m_Light->intensity > 0.f)
    {
        m_Light->intensity -= 10.f;
        co_await 0s;
    }

    coroutineId = Guid::Empty();
}
