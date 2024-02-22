#include "world.hpp"

using namespace XnorCore;


World::World()
{
    IsPlaying = true;
}

void World::Begin()
{
    Scene.Begin();
}

void World::Update()
{
    Scene.Update();
}
