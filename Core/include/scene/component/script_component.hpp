#pragma once

#include "core.hpp"
#include "Coral/ManagedObject.hpp"
#include "scene/component.hpp"
#include "utils/serializable.hpp"

/// @file script_component.hpp
/// @brief Defines the XnorCore::ScriptComponent class.

BEGIN_XNOR_CORE

/// @brief Base class for custom .NET behaviors attached to @ref Entity "Entities".
class ScriptComponent : public Component
{
    REFLECTABLE_IMPL_MINIMAL_DLL(ScriptComponent)
    
#ifndef SWIG
public:
    ScriptComponent() = default;

    XNOR_ENGINE ~ScriptComponent() override;

    XNOR_ENGINE void Initialize(const Coral::ManagedObject& managedObject);

    XNOR_ENGINE void Begin() override;

    XNOR_ENGINE void Update() override;
    
private:
    Coral::ManagedObject m_ManagedObject;
#endif
};

END_XNOR_CORE

/// @private
REFL_AUTO(
    type(XnorCore::ScriptComponent, bases<XnorCore::Component>)
);
