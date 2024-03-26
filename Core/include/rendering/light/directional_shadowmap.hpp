#pragma once

#include "core.hpp"
#include "resource/texture.hpp"

BEGIN_XNOR_CORE
struct DirectionalShadowMap
{
    Texture* depthTexture {nullptr};
};

END_XNOR_CORE
