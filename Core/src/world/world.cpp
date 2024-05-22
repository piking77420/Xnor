﻿#include "world/world.hpp"

#include "input/time.hpp"
#include "physics/physics_world.hpp"
#include "world/scene_graph.hpp"

using namespace XnorCore;

void World::Begin()
{
    scene->Begin();
}

void World::Update()
{
    if (!hasStarted)
    {
        Begin();
        hasStarted = true;
    }

    if (isPlaying)
    {
        scene->Update();
    }
    
    SceneGraph::Update(scene->GetEntities());
    
    if (isPlaying)
    {
        scene->PrePhysics();
        PhysicsWorld::Update(Time::GetDeltaTime());
        scene->PostPhysics();
    }
    scene->OnRendering();
}


