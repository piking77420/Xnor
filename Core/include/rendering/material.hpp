#pragma once

#include <vector>

#include "core.hpp"
#include "resource/texture.hpp"
#include "utils/pointer.hpp"
#include "utils/serializable.hpp"

/// @file material.hpp
/// @brief Defines types related to materials

BEGIN_XNOR_CORE

/// @brief Material type
enum class MaterialType
{
    Opaque,
    Litt
};

/// @brief Encapsulates a material, which defines how a @ref Model is rendered
struct Material : Serializable
{
    REFLECTABLE_IMPL_MINIMAL(Material)
    
public:
    // Pointer<Shader> shader;
    /// @brief Material type
    MaterialType materialType = MaterialType::Opaque;
    /// @brief Albedo texture
    Pointer<Texture> albedo;
    /// @brief Normal map
    Pointer<Texture> normalMap;
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::Material),
    field(materialType),
    field(albedo),
    field(normalMap)
);
