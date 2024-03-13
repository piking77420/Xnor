﻿#pragma once

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
/// @brief Defines the XnorCore::Entity class.

BEGIN_XNOR_CORE

/// @brief Represents an object of the engine, behaviors can be attached to it via a list of Component
class Entity : public Serializable
{
    REFLECTABLE_IMPL_H(Entity)

public:
    /// @brief Transform of the entity
    Transform transform;
    /// @brief Name of the entity
    std::string name;

    /// @brief Adds a Component to the entity
    /// @tparam T Component type
    /// @return Created component
    template <Concepts::ComponentT T>
    T* AddComponent();

    /// @brief Gets a specified Component
    /// @tparam T Component type
    /// @return Component, @c nullptr if it doesn't exists
    template <Concepts::ComponentT T>
    [[nodiscard]]
    T* GetComponent();

    /// @brief Gets a specified Component
    /// @tparam T Component type
    /// @return Component, @c nullptr if it doesn't exists
    template <Concepts::ComponentT T>
    [[nodiscard]]
    const T* GetComponent() const;
    
    /// @brief Gets all of the specified Component
    /// @tparam T Component type
    /// @param components Result components
    template <Concepts::ComponentT T>
    void GetComponents(std::vector<T*>* components);

    /// @brief Gets all of the specified Component
    /// @tparam T Component type
    /// @param components Result components
    template <Concepts::ComponentT T>
    void GetComponents(std::vector<const T*>* components) const;

    /// @brief Tries to get a component
    /// @tparam T Component type
    /// @param output Found Component
    /// @return Whether the Component exists
    template <Concepts::ComponentT T>
    [[nodiscard]]
    bool_t TryGetComponent(T** output);

    /// @brief Removes a specified Component
    /// @tparam T Component type
    template <Concepts::ComponentT T>
    void RemoveComponent();

    /// @brief Gets the Guid of the entity
    /// @return Guid
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

    /// @brief Compares 2 entities using their Guid
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

END_XNOR_CORE

#include "scene/entity.inl"

REFL_AUTO(
    type(XnorCore::Entity),
    field(name),
    field(m_EntityId, XnorCore::Reflection::HideInInspector()),
    field(transform),
    field(m_Components),
    field(m_Parent, XnorCore::Reflection::HideInInspector()),
    field(m_Children, XnorCore::Reflection::HideInInspector())
)
