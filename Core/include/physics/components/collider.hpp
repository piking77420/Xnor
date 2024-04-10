#pragma once

#include "core.hpp"
#include "physics/data/collision_data.hpp"
#include "scene/component.hpp"
#include "utils/event.hpp"

/// @file collider.hpp
/// @brief Defines the XnorCore::Collider class

BEGIN_XNOR_CORE

/// @brief Base class for collider components
class Collider : public Component
{
    REFLECTABLE_IMPL(Collider)

public:
    /// @brief Collider constraints
    enum ColliderConstraints : size_t
    {
        /// @brief No constraints
        ConstraintNone = 0 << 0,
        /// @brief Position constraint
        ConstraintPosition = 1 << 0,
        /// @brief Rotation constraint
        ConstraintRotation = 1 << 1,
    };

    XNOR_ENGINE Collider(); 
    XNOR_ENGINE ~Collider() override;

    DEFAULT_COPY_MOVE_OPERATIONS(Collider)

    /// @brief Begin function
    XNOR_ENGINE void Begin() override;
    /// @brief Update function
    XNOR_ENGINE void Update() override;
    /// @brief Pre-function
    XNOR_ENGINE void PrePhysics() override;
    /// @brief Post-function
    XNOR_ENGINE void PostPhysics() override;

    /// @brief Get whether the collider is a trigger
    /// @returns Is trigger
    [[nodiscard]]
    XNOR_ENGINE bool_t IsTrigger() const;

    /// @brief Adds a force to the component
    /// @param force Force
    XNOR_ENGINE void AddForce(const Vector3& force) const;

    /// @brief Callback if the collider is a trigger and another collider entered in it
    Event<Collider*, Collider*, const CollisionData&> onTriggerEnter;
    /// @brief Callback if the collider is a trigger and another collider stayed in it
    Event<Collider*, Collider*, const CollisionData&> onTriggerStay;
    /// @brief Callback if the collider is a trigger and another collider left it
    Event<Collider*, Collider*> onTriggerExit;
    /// @brief Callback if the collider is a collider and another collider entered in it
    Event<Collider*, Collider*, const CollisionData&> onCollisionEnter;
    /// @brief Callback if the collider is a collider and another collider stayed in it
    Event<Collider*, Collider*, const CollisionData&> onCollisionStay;
    /// @brief Callback if the collider is a collider and another collider left in it
    Event<Collider*, Collider*> onCollisionExit;

    /// @brief Collider constraints
    ColliderConstraints constraints = ConstraintNone;

protected:
    /// @brief Adds the default debug events for the callbacks
    void AddDebugEvents();

    static void Test(Collider*, Collider*, const CollisionData&) {}

    static void Test2() {}

    /// @brief Jolt internal body ID for the collider
    uint32_t m_BodyId = std::numeric_limits<uint32_t>::max();

    /// @brief Whether the collider is static
    bool_t m_IsStatic = false;
    /// @brief Whether the collider is a trigger
    bool_t m_IsTrigger = false;
    /// @brief Whether the collider is active
    bool_t m_IsActive = false;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Collider, bases<XnorCore::Component>),
    field(constraints, XnorCore::Reflection::EnumFlags()),
    field(m_IsStatic),
    field(m_IsTrigger),
    field(m_IsActive, XnorCore::Reflection::ReadOnly()),
    field(onTriggerEnter, XnorCore::Reflection::NotSerializable())
)
