#include "world.hpp"

using namespace XnorCore;


World::World()
{
}

void World::Begin()
{
    Scene.Begin();
}

void World::Update()
{
    Scene.Update();
}
