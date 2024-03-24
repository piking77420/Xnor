#pragma once

#include "bloom_rendertarget.hpp"
#include "core.hpp"

BEGIN_XNOR_CORE
class PostprocessRendertarget
{
public:

    XNOR_ENGINE void Init(Vector2i viewPortsize);

    XNOR_ENGINE void Destroy();

    XNOR_ENGINE void Resize(Vector2i viewPortsize);
    
    BloomRenderTarget bloomRenderTarget;
private:
};

END_XNOR_CORE