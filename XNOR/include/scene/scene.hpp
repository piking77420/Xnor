#pragma once
#include <vector>
#include "entity.hpp"

class Entity;

class Scene
{
public:

    static inline Scene* scene = nullptr;
    
    void Begin();

    void Update();
    
    ~Scene();
    
private:
    std::vector<Entity*> m_entities;
};
