#pragma once

#include "core.hpp"
#include "utils/serializable.hpp"

/// @file component.hpp
/// @brief Defines the XnorCore::Component class.

BEGIN_XNOR_CORE

class Entity;

/// @brief Represents a behavior that can be attached to an Entity.
class XNOR_ENGINE Component : public Serializable
{
    REFLECTABLE_IMPL_MINIMAL(Component)
    
public:
    /// @brief Entity bound to the component
    Entity* entity = nullptr;

    Component() = default;
    
    virtual ~Component() override = 0;

    DEFAULT_COPY_MOVE_OPERATIONS(Component)

    /// @brief Begins the component
    virtual void Begin() {}

    /// @brief Updates the component
    virtual void Update() {}
};

namespace Concepts
{
    /// @brief Concept that forces a type to be a child of Component
    template <class T>
    concept ComponentT = Meta::IsBaseOf<Component, T>;
}

END_XNOR_CORE

/// @private
REFL_AUTO(type(XnorCore::Component));
