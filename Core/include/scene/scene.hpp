#pragma once

#include <vector>

#include "core.hpp"
#include "entity.hpp"

BEGIN_XNOR_CORE

class Scene
{
public:
    // TO DO test to virtual memory between dll and editor exe
    // REMOVE IT LATER
    XNOR_ENGINE static void CreateBasicScene();

    // If XNOR_ENGINE so you can export it to the editor
    XNOR_ENGINE static inline Scene* scene = nullptr;

    XNOR_ENGINE ~Scene() = default;

    XNOR_ENGINE void Begin();

    XNOR_ENGINE void Update();

    XNOR_ENGINE Entity* GetEntityById(const Guid& xnorGuid);
    
private:
    std::vector<Entity> m_Entities;
};

END_XNOR_CORE
