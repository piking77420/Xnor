#pragma once

#include "core.hpp"
#include "scene/entity.hpp"

/// @file scene_graph.hpp
/// @brief Defines the SceneGraph class

BEGIN_XNOR_CORE

/// @brief Provides functions to handle parent/child entity transformation hierarchy
namespace SceneGraph
{
    /// @brief Updates the transformation of a list of entities
    /// @param entities List of entities
    XNOR_ENGINE void Update(const List<Entity*>& entities);

    /// @brief Updates an entity when its parent changed
    /// @param entity Entity
    XNOR_ENGINE void OnAttachToParent(Entity& entity);

    XNOR_ENGINE void UpdateTransform(Entity& entity);
}

END_XNOR_CORE
