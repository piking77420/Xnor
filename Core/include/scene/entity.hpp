#pragma once

#include <vector>

#include "component.hpp"
#include "core.hpp"
#include "transform.hpp"
#include "resource/model.hpp"
#include "utils/guid.hpp"
#include "utils/list.hpp"
#include "utils/poly_ptr.hpp"
#include "utils/serializable.hpp"

/// @file entity.hpp
/// @brief Defines the Entity class.

BEGIN_XNOR_CORE

/// @brief Concept that forces a type to be a child of @ref Component
template <class T>
concept ComponentT = Meta::IsBaseOf<Component, T>;

/// @brief Represents an object of the engine, behaviors can be attached to it via a list of @ref Component
class Entity : public Serializable
{
    REFLECTABLE_IMPL_H(Entity)

public:
    /// @brief Transform of the entity
    Transform transform;
    /// @brief Name of the entity
    std::string name;

    /// @brief Adds a @ref Component to the entity
    /// @tparam ComponentT Component type
    /// @return Created component
    template <class ComponentT>
    ComponentT* AddComponent();

    /// @brief Gets a specified @ref Component
    /// @tparam ComponentT Component type
    /// @return Component, @c nullptr if it doesn't exists
    template <class ComponentT>
    [[nodiscard]]
    ComponentT* GetComponent();

    /// @brief Gets a specified @ref Component
    /// @tparam ComponentT Component type
    /// @return Component, @c nullptr if it doesn't exists
    template <class ComponentT>
    [[nodiscard]]
    const ComponentT* GetComponent() const;
    
    /// @brief Gets all of the specified @ref Component
    /// @tparam ComponentT Component type
    /// @param components Result components
    template <class ComponentT>
    void GetComponents(std::vector<ComponentT*>* components);

    /// @brief Gets all of the specified @ref Component
    /// @tparam ComponentT Component type
    /// @param components Result components
    template <class ComponentT>
    void GetComponents(std::vector<const ComponentT*>* components) const;

    /// @brief Tries to get a component
    /// @tparam ComponentT Component type
    /// @param output Found @ref Component
    /// @return Whether the @ref Component exists
    template <class ComponentT>
    [[nodiscard]]
    bool_t TryGetComponent(ComponentT** output);

    /// @brief Removes a specified @ref Component
    /// @tparam ComponentT Component type
    template <class ComponentT>
    void RemoveComponent();

    /// @brief Gets the @ref Guid of the entity
    /// @return @ref Guid
    [[nodiscard]]
    XNOR_ENGINE const Guid& GetGuid() const;

    /// @brief Gets the parent of the entity
    /// @return Parent, can be @c nullptr
    [[nodiscard]]
    XNOR_ENGINE Entity* GetParent() const;

    /// @brief Gets whether the entity has a parent
    /// @return Has parent
    [[nodiscard]]
    XNOR_ENGINE bool_t HasParent() const;

    /// @brief Gets a child at a given index
    /// @param index Index
    /// @return Child
    [[nodiscard]]
    XNOR_ENGINE Entity* GetChild(size_t index) const;

    /// @brief Gets the number of children of this entity
    /// @return Children count
    [[nodiscard]]
    XNOR_ENGINE size_t GetChildCount() const;

    /// @brief Gets whether this entity has children
    /// @return Has children
    [[nodiscard]]
    XNOR_ENGINE bool_t HasChildren() const;

    /// @brief Checks if the entity is the parent of the provided entity
    /// @param child Child entity
    /// @return If this is any parent of child
    [[nodiscard]]
    XNOR_ENGINE bool_t IsAParentOf(const Entity* child) const;

    /// @brief Sets the parent of the entity
    /// @param parent New parent, use @c nullptr to orphan it
    XNOR_ENGINE void SetParent(Entity* parent);

    /// @brief Adds a child to the entity
    /// @param child Child
    XNOR_ENGINE void AddChild(Entity* child);

    /// @brief Removes a child from the entity
    /// @param child Child
    XNOR_ENGINE void RemoveChild(Entity* child);

    /// @brief Begins behavior for the entity
    XNOR_ENGINE void Begin();

    /// @brief Updates the entity
    XNOR_ENGINE void Update();

    /// @brief Compares 2 entities using their @ref Guid
    /// @param entity Other
    /// @return Equals
    XNOR_ENGINE bool_t operator==(const Entity& entity) const;
    
private:
    XNOR_ENGINE explicit Entity(const Guid& entiyId);

    XNOR_ENGINE Entity();

    XNOR_ENGINE ~Entity() override;
    
    Entity* m_Parent = nullptr;
    List<Entity*> m_Children;
    
    Guid m_EntityId;

    List<PolyPtr<Component>> m_Components;

    friend class Scene;
};

template <class ComponentT>
ComponentT* Entity::AddComponent()
{
    m_Components.Add();
    
    ComponentT* newT = new ComponentT;
    newT->entity = this;
    m_Components[m_Components.GetSize() - 1].Create(newT);

    return newT;
}

template <class ComponentT>
const ComponentT* Entity::GetComponent() const
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (m_Components[i].IsOfType<ComponentT>())
            return m_Components[i].Cast<ComponentT>();
    }

    return nullptr;
}

template <class ComponentT>
void Entity::GetComponents(std::vector<ComponentT*>* components)
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (m_Components[i].IsOfType<ComponentT>())
            components->push_back(m_Components[i].Cast<ComponentT>());
    }
}

template <class ComponentT>
void Entity::GetComponents(std::vector<const ComponentT*>* components) const
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (m_Components[i].IsOfType<ComponentT>())
            components->push_back(reinterpret_cast<const ComponentT*>(m_Components[i]));
    }
}

template <class ComponentT>
ComponentT* Entity::GetComponent()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (m_Components[i].IsOfType<ComponentT>())
            return m_Components[i].Cast<ComponentT>();
    }
    
    return nullptr;
}

template <class ComponentT>
void Entity::RemoveComponent()
{
    for (int i = 0; i < m_Components.GetSize(); i++)
    {
        if (m_Components[i].IsOfType<ComponentT>())
        {
            m_Components.RemoveAt(i);
            break;
        }
    }
}

template <class ComponentT>
bool_t Entity::TryGetComponent(ComponentT** output)
{
    for (int i = 0; i < m_Components.GetSize(); i++)
    {
        if (m_Components[i].IsOfType<ComponentT>())
        {
            *output = m_Components[i].Cast<ComponentT>();
            return true;
        }
    }
    
    return false;
}

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::Entity),
    field(name),
    field(m_EntityId, XnorCore::HideInInspector()),
    field(transform),
    field(m_Components),
    field(m_Parent, XnorCore::HideInInspector()),
    field(m_Children, XnorCore::HideInInspector())
)
