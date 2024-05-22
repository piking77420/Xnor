#include "physics/component/box_collider.hpp"

#include "physics/physics_world.hpp"
#include "physics/component/collider.hpp"
#include "scene/entity.hpp"
#include "serialization/serializer.hpp"

using namespace XnorCore;

void BoxCollider::Begin()
{
   
}

void BoxCollider::Update()
{
    Collider::Update();
}
