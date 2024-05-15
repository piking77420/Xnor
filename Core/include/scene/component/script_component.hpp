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

    /// @brief Initializes this ScriptComponent with the given ManagedObject, effectively linking it with its instantiated .NET version.
    XNOR_ENGINE void Initialize(const Coral::ManagedObject& managedObject);

    /// @brief Destroys this ScriptComponent in the .NET garbage collector.
    XNOR_ENGINE void Destroy() override;

    XNOR_ENGINE void Begin() override;

    XNOR_ENGINE void Update() override;

    /// @brief Returns the .NET ManagedObject linked to this ScriptComponent
    [[nodiscard]]
    XNOR_ENGINE Coral::ManagedObject& GetManagedObject();

    /// @brief Returns the .NET ManagedObject linked to this ScriptComponent
    [[nodiscard]]
    XNOR_ENGINE const Coral::ManagedObject& GetManagedObject() const;
    
private:
    Coral::ManagedObject m_ManagedObject;

    // The DotnetRuntime class needs to have access to the m_ManagedObject field
    friend class DotnetRuntime;
#endif
};

END_XNOR_CORE

/// @private
REFL_AUTO(
    type(XnorCore::ScriptComponent, bases<XnorCore::Component>)
);
