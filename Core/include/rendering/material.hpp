#pragma once

#include <vector>

#include "core.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_CORE

struct Material
{
    Pointer<Shader> shader;
    std::vector<Pointer<Texture>> textures;
};

END_XNOR_CORE
