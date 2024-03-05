#pragma once

#include <vector>

#include "core.hpp"
#include "entity.hpp"

/// @file scene.hpp
/// @brief Defines the Scene class.
 
BEGIN_XNOR_CORE

/// @brief Represents a scene, encapsulates a List of Entity and provides utility functions to manipulate said entities
class Scene : public Serializable
{
    REFLECTABLE_IMPL_H(Scene)
    
public:
    XNOR_ENGINE Scene() = default;

    XNOR_ENGINE ~Scene() override;

    DEFAULT_COPY_MOVE_OPERATIONS(Scene)

    /// @brief Gets all of the specified @ref Component in every entity in the scene
    /// @tparam ComponentT Component type
    /// @param components Result components
    template <class ComponentT>
    void GetAllComponentOfType(std::vector<const ComponentT*>* components) const;

    /// @brief Gets all of the specified @ref Component in every entity in the scene
    /// @tparam ComponentT Component type
    /// @param components Result components
    template <class ComponentT>
    void GetAllComponentOfType(std::vector<ComponentT*>* components);

    /// @brief Begins the scene
    XNOR_ENGINE void Begin();

    /// @brief Updates the scene
    XNOR_ENGINE void Update();

    /// @brief Tries to find an entity in the scene via a @ref Guid
    /// @param xnorGuid Guid
    /// @return Entity, can be @c nullptr
    [[nodiscard]]
    XNOR_ENGINE Entity* GetEntityById(const Guid& xnorGuid);

    /// @brief Creates a new entity in the scene
    /// @param name Entity name
    /// @param parent Entity parent, no parent by default
    /// @return Created entity
    XNOR_ENGINE Entity* CreateEntity(const std::string&& name, Entity* parent = nullptr);

    /// @brief Destroys an entity and removes it from the scene
    /// @param entity Entity
    XNOR_ENGINE void DestroyEntity(Entity* entity);

    /// @brief Checks if the scene contains a specified entity
    /// @param entity Entity
    /// @return Whether the scene contains the entity
    [[nodiscard]]
    XNOR_ENGINE bool_t HasEntity(const Entity* entity) const;

    /// @brief Gets the @ref List of @ref Entity of the scene
    /// @return @ref List of @ref Entity
    [[nodiscard]]
    XNOR_ENGINE const List<Entity*>& GetEntities();

private:
    List<Entity*> m_Entities;

    XNOR_ENGINE void DestroyEntityChildren(Entity* entity);
};

template <class ComponentT>
void Scene::GetAllComponentOfType(std::vector<const ComponentT*>* const components) const
{
    for (size_t i = 0; i < m_Entities.GetSize(); i++)
    {
        const ComponentT* component = m_Entities[i]->GetComponent<ComponentT>();

        if (component != nullptr)
            components->push_back(component);
    }
}

template <class ComponentT>
void Scene::GetAllComponentOfType(std::vector<ComponentT*>* const components)
{
    for (size_t i = 0; i < m_Entities.GetSize(); i++)
    {
        ComponentT* component = m_Entities[i]->GetComponent<ComponentT>();

        if (component != nullptr)
            components->push_back(component);
    }
}

END_XNOR_CORE

/// @private
REFL_AUTO(type(XnorCore::Scene),
    field(m_Entities, XnorCore::ExpandPointer(), XnorCore::HideInInspector())
);
