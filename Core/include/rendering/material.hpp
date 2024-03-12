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
enum class MaterialType : uint8_t
{
    Opaque,
    Lit
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
    
    Colorf albedoColor;
    float_t metallic = 0.f;
    float_t roughness = 0.f;
    float_t reflectance = 0.f;
    float_t emissive = 0.f;
    float_t ambientOcclusion = 0.f;


};

END_XNOR_CORE

/// @private
REFL_AUTO(
    type(XnorCore::Material),
    field(materialType),
    field(metallic),
    field(roughness),
    field(reflectance),
    field(emissive),
    field(ambientOcclusion),
    field(albedo),
    field(normalMap),
    field(albedoColor)
);
