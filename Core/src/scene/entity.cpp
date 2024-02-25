#include "scene/entity.hpp"

#include "scene/component.hpp"
#include "scene/scene.hpp"
#include "serialization/serializer.hpp"

using namespace XnorCore;

Entity::Entity(const Guid& entiyId)
    : m_EntityId(entiyId)
{
}

Entity::Entity()
{
    Entity::CreateTypeInfo();
    transform.CreateTypeInfo();
}

Entity::~Entity()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        PolyPtr<Component>::Destroy(&m_Components[i]);
    }

    m_Components.Clear();
}

void Entity::Begin()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        m_Components[i]->Begin();
    }
}

void Entity::Update()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        m_Components[i]->Update();
    }
}

const Guid& Entity::GetId() const
{
    return m_EntityId;
}

Entity* Entity::GetParent() const
{
    return m_Parent;
}

bool Entity::HasParent() const
{
    return m_Parent != nullptr;
}

Entity* Entity::GetChild(const size_t index) const
{
    return m_Children[index];
}

size_t Entity::GetChildCount() const
{
    return m_Children.size();
}

bool Entity::HasChildren() const
{
    return GetChildCount() != 0;
}

bool Entity::IsAParentOf(const Entity* child) const
{
    const Entity* e = child->m_Parent;

    while (e)
    {
        if (e == this)
            return true;

        e = e->m_Parent;
    }

    return false;
}

void Entity::SetParent(Entity* const parent)
{
    // Remove ourselves from our old parent if we had one
    if (HasParent())
        m_Parent->m_Children.erase(std::ranges::find(m_Parent->m_Children, this));

    // Set new parent
    m_Parent = parent;

    // Need to check if we actually have a parent, since a nullptr parent is valid
    if (parent)
    {
        // Add ourselves to our new parent
        parent->m_Children.push_back(this);
    }
}

void Entity::AddChild(Entity* child)
{
    if (child == nullptr)
    {
        Logger::LogWarning("Trying to add a child that doesn't exists");
        return;
    }

    // Add the enw child
    m_Children.push_back(child);

    // Check if the child add a parent
    if (child->HasParent())
    {
        // If it had one, remove its old child affiliation
        m_Parent->m_Children.erase(std::ranges::find(m_Parent->m_Children, this));
    }

    // Set the new parent of the child to ourselves
    child->m_Parent = this;
}

void Entity::RemoveChild(Entity* const child)
{
    if (child == nullptr)
    {
        Logger::LogWarning("Trying to remove a child that doesn't exists");
        return;
    }
    
    // Remove from the list if the parent was indeed us
    if (child->m_Parent == this)
        m_Children.erase(std::ranges::find(m_Children, child));

    // Orphan the child
    child->m_Parent = nullptr;
}

bool Entity::operator==(const Entity& entity) const
{
    return m_EntityId == entity.m_EntityId;
}

void Entity::Serialize() const
{
    Serializer::BeginXmlElement("Entity", std::format("{}", m_EntityId));
    transform.Serialize();
    Serializer::EndXmlElement();
}

void Entity::Deserialize()
{
    
}

