#pragma once

#include <vector>

#include "entity.hpp"
#include "core.hpp"

class Scene
{
public:
    // TO DO test to virtual memory between dll and editor exe
    // REMOVE IT LATER
    XNOR_ENGINE static void CreateBasicScene();

    // If XNOR_ENGINE is place you can export hit to editor
    XNOR_ENGINE static inline Scene* scene = nullptr;

    XNOR_ENGINE ~Scene();

    XNOR_ENGINE void Begin();

    XNOR_ENGINE void Update();

    Entity* GetEntityById(const Guid& xnorGuid);
    
private:
    std::vector<Entity> m_Entities;
};
