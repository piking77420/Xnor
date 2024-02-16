#pragma once

#include "core.hpp"
#include "resource/texture.hpp"
#include "utils/pointer.hpp"
#include "resource/shader.hpp"


BEGIN_XNOR_CORE
struct Material
{
    Pointer<Shader> shader;
    std::vector<Pointer<Texture>> textures;
};

END_XNOR_CORE