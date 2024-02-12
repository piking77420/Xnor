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

    // If XNOR_ENGINE so you can export it to the editor
    XNOR_ENGINE static inline Scene* scene = nullptr;

    XNOR_ENGINE ~Scene();

    XNOR_ENGINE void Begin();

    XNOR_ENGINE void Update();

    XNOR_ENGINE Entity* GetEntityById(const Guid& xnorGuid);
    
private:
    std::vector<Entity> m_Entities;
};
