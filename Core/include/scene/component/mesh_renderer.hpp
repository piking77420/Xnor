#pragma once

#include "core.hpp"
#include "rendering/material.hpp"
#include "resource/model.hpp"
#include "scene/component.hpp"

/// @file mesh_renderer.hpp
/// @brief Defines the XnorCore::MeshRenderer class.

BEGIN_XNOR_CORE

/// @brief Component that defines a mesh to render
class MeshRenderer : public Component
{
    REFLECTABLE_IMPL_MINIMAL_DLL(MeshRenderer)
    
public:
    XNOR_ENGINE MeshRenderer() = default;

    /// @brief @ref Model of the mesh
    Pointer<Model> model;

    /// @brief @ref Material to render with
    Material material;

    /// @brief Whether to draw the model AABB box
    bool_t drawModelAabb = false;
};

END_XNOR_CORE

/// @private
REFL_AUTO(
    type(XnorCore::MeshRenderer, bases<XnorCore::Component>),
    field(model),
    field(material),
    field(drawModelAabb)
);
