#pragma once

#include "core.hpp"
#include "hierarchy.hpp"
#include "scene/scene.hpp"

BEGIN_XNOR_CORE


class World
{
public:
    XNOR_ENGINE static inline World* world = nullptr;
    
    bool isPlaying = false;
    
    bool hasStarted = false;
    
    Scene Scene;

    Hierarchy hierarchy;
    
    XNOR_ENGINE World();

    XNOR_ENGINE ~World() = default;
    
    XNOR_ENGINE void Begin();
    
    XNOR_ENGINE void Update();


private:
};

END_XNOR_CORE
