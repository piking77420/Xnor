#pragma once

#include "core.hpp"
#include "rendering/material.hpp"
#include "resource/mesh.hpp"
#include "resource/model.hpp"
#include "scene/component.hpp"

/// @file mesh_renderer.hpp
/// @brief Defines the XnorCore::MeshRenderer class.

BEGIN_XNOR_CORE

/// @brief Component that defines a mesh to render
class SkinnedMeshRender : public Component
{
    REFLECTABLE_IMPL(SkinnedMeshRender)
    
public:
    XNOR_ENGINE SkinnedMeshRender() = default;
    
    /// @brief @ref Mesh
    Pointer<Mesh> mesh;

    /// @brief @ref Material to render with
    Material material;

    /// @brief Whether to draw the model AABB box
    bool_t drawModelAabb = false;
};

END_XNOR_CORE

/// @private
REFL_AUTO(
    type(XnorCore::SkinnedMeshRender, bases<XnorCore::Component>),
    field(mesh),
    field(material),
    field(drawModelAabb)
);
