#pragma once

#include "core.hpp"
#include "reflection/reflection.hpp"
#include "scene/component.hpp"

/// @file script_component.hpp
/// @brief Defines the XnorCore::ScriptComponent class.

BEGIN_XNOR_CORE

/// @brief Base class for custom behaviors attached to @ref Entity "Entities".
class ScriptComponent : public Component
{
    REFLECTABLE_IMPL(ScriptComponent)

public:
};

END_XNOR_CORE

/// @private
REFL_AUTO(
    type(XnorCore::ScriptComponent, bases<XnorCore::Component>)
);
