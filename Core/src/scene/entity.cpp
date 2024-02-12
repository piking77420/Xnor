#include "entity.hpp"

#include "scene.hpp"
#include "component.hpp"

Entity::Entity(const Guid& entiyID) : m_entityId(entiyID)
{

}

Entity::Entity()
{
   
}

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

bool Entity::operator==(const Entity& entity)
{
    return m_entityId == entity.m_entityId;
}

