#pragma once

#include "core.hpp"
#include "utils/serializable.hpp"

/// @file component.hpp
/// @brief Defines the @ref Component class.

BEGIN_XNOR_CORE

class Entity;

/// @brief Represents a behavior that can be attached to an @ref Entity
class XNOR_ENGINE Component : public Serializable
{
    REFLECTABLE_IMPL_MINIMAL(Component)
    
public:
    /// @brief Entity bound to the component
    Entity* entity = nullptr;

    Component() = default;
    
    ~Component() override = default;

    /// @brief Begins the component
    virtual void Begin() {}

    /// @brief Updates the component
    virtual void Update() {}
};

END_XNOR_CORE

/// @private
REFL_AUTO(type(XnorCore::Component));
