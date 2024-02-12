#include "entity.hpp"

#include "scene.hpp"
#include "component.hpp"

using namespace XnorCore;

Entity::Entity(const Guid& entiyId)
    : m_EntityId(entiyId)
{
}

Entity::~Entity()
{
    for (const Component* comp : m_Components)
    {
       delete comp;
    }

    m_Components.clear();
}

void Entity::Begin() const
{
    for (Component* comp : m_Components)
    {
        comp->Begin();
    }
}

void Entity::Update() const
{
    for (Component* comp : m_Components)
    {
        comp->Update();
    }
}

bool Entity::operator==(const Entity& entity) const
{
    return m_EntityId == entity.m_EntityId;
}

