#pragma once

#include "core.hpp"
#include "Coral/ManagedObject.hpp"
#include "reflection/reflection.hpp"
#include "scene/component.hpp"

/// @file script_component.hpp
/// @brief Defines the XnorCore::ScriptComponent class.

BEGIN_XNOR_CORE

/// @brief Base class for custom .NET behaviors attached to @ref Entity "Entities".
class ScriptComponent : public Component
{
#ifndef SWIG
    REFLECTABLE_IMPL(ScriptComponent)

public:
    ScriptComponent() = default;

    XNOR_ENGINE ~ScriptComponent() override;

    XNOR_ENGINE void Initialize(const Coral::ManagedObject& managedObject);

    XNOR_ENGINE void Begin() override;

    XNOR_ENGINE void Update() override;

    XNOR_ENGINE Coral::ManagedObject& GetManagedObject();
    
private:
    Coral::ManagedObject m_ManagedObject;
#endif
};

END_XNOR_CORE

/// @private
REFL_AUTO(
    type(XnorCore::ScriptComponent, bases<XnorCore::Component>)
);
