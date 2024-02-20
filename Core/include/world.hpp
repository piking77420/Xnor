#pragma once

#include "core.hpp"
#include "scene/scene.hpp"

BEGIN_XNOR_CORE


class World
{
public:
    
    XNOR_ENGINE static inline World* world = nullptr;
    
    bool IsPlaying = false;
    
    bool HasBegin = false;
    
    Scene Scene;
    
    XNOR_ENGINE World();

    XNOR_ENGINE ~World() = default;
    
    XNOR_ENGINE void Begin();
    
    XNOR_ENGINE void Update();
  
private:
};

END_XNOR_CORE