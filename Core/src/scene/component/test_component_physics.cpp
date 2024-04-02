#include "scene/component/test_component_physics.hpp"

#include "input/input.hpp"
#include "input/time.hpp"
#include "physics/components/collider.hpp"
#include "scene/entity.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void TestComponentPhysics::Begin()
{
    m_Collider = entity->GetComponent<Collider>();
}

void TestComponentPhysics::Update()
{
    constexpr float_t speed = 12.f;
    
    Transform& t = entity->transform;
    const Vector3 position = t.GetPosition();

    if (Input::GetKey(Key::Up))
        t.SetPositionZ(position.z + speed * Time::GetDeltaTime());

    if (Input::GetKey(Key::Down))
        t.SetPositionZ(position.z - speed * Time::GetDeltaTime());

    if (Input::GetKey(Key::Right))
        t.SetPositionX(position.x + speed * Time::GetDeltaTime());

    if (Input::GetKey(Key::Left))
        t.SetPositionX(position.x - speed * Time::GetDeltaTime());

    if (m_Collider)
    {
        if (Input::GetKey(Key::Space, KeyStatus::Pressed))
            m_Collider->AddForce(Vector3::UnitY() * m_JumpForce);
    }
}
