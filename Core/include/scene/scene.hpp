#pragma once

#include <vector>

#include "entity.hpp"
#include "core.hpp"

class Scene
{
public:
    static inline Scene* scene = nullptr;

    XNOR_ENGINE void Begin();

    XNOR_ENGINE void Update();
    
    XNOR_ENGINE ~Scene();
    
private:
    std::vector<Entity*> m_entities;
};
