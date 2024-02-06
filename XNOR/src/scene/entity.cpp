#include "entity.hpp"
#include "scene.hpp"

Entity::~Entity()
{
    for (Component* comp : m_Components)
    {
       delete comp;
    }

    m_Components.clear();
}

void Entity::Begin()
{
    for (Component* comp : m_Components)
    {
        comp->Begin();
    }
}

void Entity::Update()
{
    for (Component* comp : m_Components)
    {
        comp->Update();
    }
}
