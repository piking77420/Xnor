#include "scene/component/test_component_physics.hpp"

#include "input/input.hpp"
#include "physics/components/collider.hpp"
#include "scene/entity.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void TestComponentPhysics::Begin()
{
}

void TestComponentPhysics::Update()
{
    Transform& t = entity->transform;
    const Vector3 position = t.GetPosition();

    if (Input::GetKey(Key::Up))
        t.SetPositionZ(position.z + .2f);

    if (Input::GetKey(Key::Down))
        t.SetPositionZ(position.z - .2f);

    if (Input::GetKey(Key::Right))
        t.SetPositionX(position.x + .2f);

    if (Input::GetKey(Key::Left))
        t.SetPositionX(position.x - .2f);
}
