#pragma once

#include <vector>

#include "core.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"
#include "utils/pointer.hpp"
#include "utils/reflectable.hpp"
#include "serialization/serializer.hpp"

BEGIN_XNOR_CORE
enum class MaterialType
{
    Opaque,
    Litt
};

struct Material : public Reflectable
{
    REFLECTABLE_IMPL(Material)
public:
    Material();
    
    //Pointer<Shader> shader;
    MaterialType m_MaterialType;
    Pointer<Texture> textures;
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::Material),
    field(m_MaterialType),
    field(textures)
);
