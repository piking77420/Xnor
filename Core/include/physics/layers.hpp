#pragma once

#include "core.hpp"
#include "jolt/Physics/Collision/ObjectLayer.h"

BEGIN_XNOR_CORE

/// @private
namespace Layers
{
    static constexpr JPH::ObjectLayer NON_MOVING = 0;
    static constexpr JPH::ObjectLayer MOVING = 1;
    static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
}

END_XNOR_CORE
