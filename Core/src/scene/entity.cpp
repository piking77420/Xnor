#include "scene/entity.hpp"

#include "scene/scene.hpp"
#include "scene/component.hpp"

XnorCore::Entity::Entity(const Guid& entiyId)
    : m_EntityId(entiyId)
{
}

XnorCore::Entity::~Entity()
{
    for (const Component* comp : m_Components)
    {
       delete comp;
    }

    m_Components.clear();
}

void XnorCore::Entity::Begin() const
{
    Entity::CreateTypeInfo();
    for (Component* comp : m_Components)
    {
        comp->Begin();
    }
}

void XnorCore::Entity::Update() const
{
    for (Component* comp : m_Components)
    {
        comp->Update();
    }
}

bool XnorCore::Entity::operator==(const Entity& entity) const
{
    return m_EntityId == entity.m_EntityId;
}

