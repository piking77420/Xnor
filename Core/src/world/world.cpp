#include "world/world.hpp"

#include "input/time.hpp"
#include "physics/physics_world.hpp"

using namespace XnorCore;

void World::Begin()
{
    scene->Begin();
}

void World::Update()
{
    PhysicsWorld::Update(Time::GetDeltaTime());
    scene->Update();
}
