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
class MeshRenderer : public Component
{
    REFLECTABLE_IMPL(MeshRenderer)
    
public:
    /// @brief @ref Model of the mesh
    Pointer<Model> model;

    /// @brief @ref Mesh
    Pointer<Mesh> mesh;

    /// @brief @ref Material to render with
    Material material;

    /// @brief Whether to draw the model AABB box
    bool_t drawModelAabb = false;
    
    XNOR_ENGINE MeshRenderer() = default;

    XNOR_ENGINE ~MeshRenderer() override = default ;
    
    DEFAULT_COPY_MOVE_OPERATIONS(MeshRenderer);

    XNOR_ENGINE void GetAabb(Bound* bound) const;
};

END_XNOR_CORE

/// @private
REFL_AUTO(
    type(XnorCore::MeshRenderer, bases<XnorCore::Component>),
    field(model),
    field(mesh),
    field(material),
    field(drawModelAabb)
);
