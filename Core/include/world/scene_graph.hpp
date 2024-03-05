#pragma once

#include "core.hpp"
#include "scene/entity.hpp"

/// @file scene_graph.hpp
/// @brief Defines the SceneGraph class

// TODO move the functions below to the ToolBox

/// @brief Decomposes a model matrix into its components
/// @param modelMatrix Model matrix
/// @param translation Translation result
/// @param orientation Rotation result
/// @param scale Scaling result
/// @param skew Skew result
/// @param perspective Perspective result
/// @return Whether the operation failed
XNOR_ENGINE bool_t Decompose(const Matrix& modelMatrix, Vector3* translation, Quaternion* orientation, Vector3* scale, Vector3* skew, Vector4* perspective);

/// @brief Combines 2 Vector3
/// @param a A
/// @param b B
/// @param ascl A scale
/// @param bscl B scale
/// @result Result
XNOR_ENGINE Vector3 Combine(const Vector3& a, const Vector3& b, float_t ascl, float_t bscl);

/// @brief Scales a Vector3 to a desired length
/// @param v Vector
/// @param desiredLength Desired length
/// @result Result
XNOR_ENGINE Vector3 ScaleVector(const Vector3& v, float_t desiredLength);


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
}

END_XNOR_CORE
