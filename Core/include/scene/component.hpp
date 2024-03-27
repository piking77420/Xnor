#pragma once

#include "core.hpp"
#include "reflection/reflection.hpp"

/// @file component.hpp
/// @brief Defines the XnorCore::Component class.

BEGIN_XNOR_CORE

class Entity;
class Transform;

/// @brief Represents a behavior that can be attached to an Entity.
class XNOR_ENGINE Component
{
    REFLECTABLE_IMPL(Component)

public:
    Component() = default;

    virtual ~Component() = 0;

    DEFAULT_COPY_MOVE_OPERATIONS(Component)

    /// @brief Begins the component
    virtual void Begin() {}

    /// @brief Updates the component
    virtual void Update() {}

    /// @brief Called before the physics update
    virtual void PrePhysics() {}

    /// @brief Called after the physics update
    virtual void PostPhysics() {}

    /// @brief Get the Entity on which this Component is attached
    [[nodiscard]]
    const Entity* GetEntity() const;

#ifndef SWIG
    /// @brief Get the Entity on which this Component is attached
    Entity* GetEntity();
#endif
    
    /// @brief Get the Transform attached to the Entity of this Component
    ///
    /// Shortcut for equivalent to:
    /// @code
    /// GetEntity().transform
    /// @endcode
    ///
    /// @see GetEntity
    const Transform& GetTransform() const;

#ifndef SWIG
    /// @brief Get the Transform attached to the Entity of this Component
    ///
    /// Shortcut for equivalent to:
    /// @code
    /// GetEntity().transform
    /// @endcode
    ///
    /// @see GetEntity
    Transform& GetTransform();

    // TODO m_Entity must be private at some point
    /// @brief Entity bound to the component
    Entity* entity = nullptr;
#endif
private:

    // We need Entity to be able to set m_Entity
    friend class Entity;
};

END_XNOR_CORE

/// @private
REFL_AUTO(type(XnorCore::Component));
