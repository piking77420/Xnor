#pragma once

#include <vector>

#include "core.hpp"
#include "reflection/reflection.hpp"
#include "resource/texture.hpp"
#include "utils/pointer.hpp"

/// @file material.hpp
/// @brief Defines types related to materials

BEGIN_XNOR_CORE

/// @brief Material type
enum class MaterialType
{
    Opaque,
    Lit
};

/// @brief Encapsulates a material, which defines how a @ref Model is rendered
struct Material
{
    REFLECTABLE_IMPL(Material)
    
public:
    // Pointer<Shader> shader;

    /// @brief Material type
    MaterialType materialType = MaterialType::Opaque;
    /// @brief Albedo texture
    Pointer<Texture> albedoTexture;

    Pointer<Texture> metallicTexture;

    Pointer<Texture> roughnessTexture;
    /// @brief Normal map
    Pointer<Texture> normalTexture;
    
    Pointer<Texture> ambiantOcclusionTexture;

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
    field(albedoColor),
    field(metallic, XnorCore::Reflection::Range(0.f, 1.f)),
    field(roughness, XnorCore::Reflection::Range(0.f, 1.f)),
    field(reflectance, XnorCore::Reflection::Range(0.f, 1.f)),
    field(emissive, XnorCore::Reflection::Range(0.f, 150000.f)),
    field(ambientOcclusion, XnorCore::Reflection::Range(0.f, 1.f)),
    field(albedoTexture),
    field(metallicTexture),
    field(roughnessTexture),
    field(normalTexture),
    field(ambiantOcclusionTexture)
);
