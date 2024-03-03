#pragma once

#include <vector>

#include "core.hpp"
#include "resource/texture.hpp"
#include "utils/pointer.hpp"
#include "utils/reflectable.hpp"

BEGIN_XNOR_CORE

enum MaterialType
{
    Opaque,
    Litt
};

struct Material : Reflectable
{
    REFLECTABLE_IMPL_MINIMAL(Material)
    
public:
    // Pointer<Shader> shader;
    int32_t materialType;
    Pointer<Texture> albedo;
    Pointer<Texture> normalMap;

};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::Material),
    field(materialType),
    field(albedo),
    field(normalMap)
);
