#pragma once

#include "core.hpp"
#include "rendering/animator.hpp"
#include "rendering/material.hpp"
#include "resource/mesh.hpp"
#include "resource/model.hpp"
#include "scene/component.hpp"

/// @file skinned_mesh_render.hpp
/// @brief Defines the XnorCore::SkinnedMeshRender class.

BEGIN_XNOR_CORE

/// @brief Component that defines a mesh to render
class SkinnedMeshRender final : public Component
{
    REFLECTABLE_IMPL(SkinnedMeshRender)

public:
    XNOR_ENGINE SkinnedMeshRender() = default;

    XNOR_ENGINE void Begin() override;
    XNOR_ENGINE void Update() override;

    /// @brief @ref Mesh
    Pointer<Mesh> mesh;

    /// @brief Whether to draw the model AABB box
    bool_t drawModelAabb = false;

    XNOR_ENGINE void StartBlending(const Animation* target);

    XNOR_ENGINE const List<Matrix>& GetMatrices() const;

private:
    Animator m_Animator;
    Animator m_TargetAnimator;
};

END_XNOR_CORE

/// @private
REFL_AUTO(
    type(XnorCore::SkinnedMeshRender, bases<XnorCore::Component>),
    field(mesh),
    field(drawModelAabb),
    field(m_Animator)
);
