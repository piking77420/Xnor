#pragma once

#include "core.hpp"

#include <Jolt/Jolt.h>
#include "Jolt/Physics/PhysicsSystem.h"
#include "Jolt/Core/JobSystemThreadPool.h"
#include "Maths/quaternion.hpp"
#include "physics/broad_phase_layer_interface.hpp"
#include "physics/body_activation_listener.hpp"
#include "physics/contact_listener.hpp"
#include "Maths/vector3.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE PhysicsWorld
{
    STATIC_CLASS(PhysicsWorld)

public:
    static void Initialize();
    static void Destroy();
    static void Update(float_t deltaTime);

    static void SetGravity(const Vector3& gravity);

    [[nodiscard]]
    static uint32_t CreateSphere(const Vector3& position, float_t radius);
    [[nodiscard]]
    static uint32_t CreateBox(const Vector3& position, const Quaternion& rotation, const Vector3& scale);

    static void DestroyBody(uint32_t bodyId);

    [[nodiscard]]
    static bool_t IsBodyActive(uint32_t bodyId);
    [[nodiscard]]
    static Vector3 GetBodyPosition(uint32_t bodyId);
    [[nodiscard]]
    static Quaternion GetBodyRotation(uint32_t bodyId);

    static void SetPosition(uint32_t bodyId, const Vector3& position);
    static void SetRotation(uint32_t bodyId, const Quaternion& rotation);

private:
    static void TraceImpl(const char_t* format, ...);

    static inline JPH::PhysicsSystem* m_PhysicsSystem;
    static inline JPH::TempAllocatorImpl* m_Allocator;
    static inline JPH::JobSystemThreadPool* m_JobSystem;

    static inline BroadPhaseLayerInterfaceImpl m_BroadPhaseLayerInterface;
    static inline JPH::ObjectVsBroadPhaseLayerFilter m_ObjectVsBroadphaseLayerFilter;
    static inline JPH::ObjectLayerPairFilter m_ObjectVsObjectLayerFilter;
    static inline BodyActivationListenerImpl m_BodyActivationListener;
    static inline ContactListenerImpl m_ContactListener;

    static inline JPH::BodyInterface* m_BodyInterface;
};

END_XNOR_CORE
