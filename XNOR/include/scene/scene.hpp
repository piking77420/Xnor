#pragma once
#include <vector>
#include "entity.hpp"

class Scene
{
public:

    static Scene* GetScene()
    {
        return m_CurrentScene;
    }
    
    static void Begin();

    static void Update();
    
private:
    
    static inline Scene* m_CurrentScene = nullptr;
    
public:
    ~Scene();
    
private:
    std::vector<Entity*> m_entities;
};
