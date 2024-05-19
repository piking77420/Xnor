#include "scene/entity.hpp"

#include "scene/component.hpp"
#include "scene/component/script_component.hpp"
#include "serialization/serializer.hpp"
#include "utils/logger.hpp"
#include "world/scene_graph.hpp"

using namespace XnorCore;

Entity::Entity(const Guid& entiyId)
    : m_EntityId(entiyId)
{
}

Entity::~Entity()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
        m_Components[i]->Destroy();

    m_Components.Clear();
}

void Entity::Begin()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (!m_Components[i]->enabled)
            continue;
            
        m_Components[i]->Begin();
    }
}

void Entity::Update()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (!m_Components[i]->enabled)
            continue;
        
        m_Components[i]->Update();
    }
}

void Entity::PrePhysics()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        m_Components[i]->PrePhysics();
    }
}

void Entity::PostPhysics()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        m_Components[i]->PostPhysics();
    }
}

void Entity::OnRendering()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
        m_Components[i]->OnRendering();
}

Entity* Entity::Clone() const
{
    Entity* clone = World::scene->CreateEntity(name, m_Parent);

    for (Component* component : m_Components)
    {
        // TODO: Clone components
        //clone->AddComponent<>();
    }

    for (const Entity* child : m_Children)
        clone->AddChild(child->Clone());

    return clone;
}

void Entity::AddComponent(Component* const component)
{
    component->entity = this;
    m_Components.Add(component);
}

const Guid& Entity::GetGuid() const
{
    return m_EntityId;
}

Entity* Entity::GetParent() const
{
    return m_Parent;
}

bool_t Entity::HasParent() const
{
    return m_Parent != nullptr;
}

Entity* Entity::GetChild(const size_t index) const
{
    return m_Children[index];
}

size_t Entity::GetChildCount() const
{
    return m_Children.GetSize();
}

bool_t Entity::HasChildren() const
{
    return GetChildCount() != 0;
}

bool_t Entity::IsParentOf(const Entity* child) const
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
        m_Parent->m_Children.Remove(this);

    // Set new parent
    m_Parent = parent;

    // Need to check if we actually have a parent, since a nullptr parent is valid
    if (parent)
    {
        SceneGraph::OnAttachToParent(*this);
        // Add ourselves to our new parent
        parent->m_Children.Add(this);
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
    m_Children.Add(child);

    // Check if the child add a parent
    if (child->HasParent())
    {
        // If it had one, remove its old child affiliation
        m_Parent->m_Children.Remove(this);
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
        m_Children.Remove(child);

    // Orphan the child
    child->m_Parent = nullptr;
}

bool_t Entity::operator==(const Entity& entity) const
{
    return m_EntityId == entity.m_EntityId;
}
