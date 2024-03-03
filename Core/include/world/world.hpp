#pragma once

#include "core.hpp"
#include "scene_graph.hpp"
#include "skybox.hpp"
#include "scene/scene.hpp"

BEGIN_XNOR_CORE


class World
{
public:
    XNOR_ENGINE static inline World* world = nullptr;
    
    bool isPlaying = false;
    
    bool hasStarted = false;
    
    Scene Scene;

    SceneGraph hierarchy;

    Skybox skybox;
    
    XNOR_ENGINE World();

    XNOR_ENGINE ~World() = default;
    
    XNOR_ENGINE void Begin();
    
    XNOR_ENGINE void Update();


private:
};

END_XNOR_CORE
