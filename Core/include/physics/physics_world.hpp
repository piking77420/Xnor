#pragma once

#include "core.hpp"

#include <unordered_map>

#include <Jolt/Jolt.h>

#include "components/collider.hpp"
#include "Jolt/Physics/PhysicsSystem.h"
#include "Jolt/Core/JobSystemThreadPool.h"
#include "Maths/quaternion.hpp"
#include "physics/broad_phase_layer_interface.hpp"
#include "physics/body_activation_listener.hpp"
#include "physics/contact_listener.hpp"
#include "Maths/vector3.hpp"
#include "rendering/vertex.hpp"

BEGIN_XNOR_CORE

class PhysicsWorld
{
    STATIC_CLASS(PhysicsWorld)

public:
    XNOR_ENGINE static void Initialize();
    XNOR_ENGINE static void Destroy();
    XNOR_ENGINE static void Update(float_t deltaTime);

    XNOR_ENGINE static void SetGravity(const Vector3& gravity);

    [[nodiscard]]
    XNOR_ENGINE static uint32_t CreateSphere(Collider* c, const Vector3& position, float_t radius, bool_t isTrigger, bool_t isStatic);
    [[nodiscard]]
    XNOR_ENGINE static uint32_t CreateBox(Collider* c, const Vector3& position, const Quaternion& rotation, const Vector3& scale, bool_t isTrigger, bool_t isStatic);
    [[nodiscard]]
    XNOR_ENGINE static uint32_t CreateCapsule(Collider* c, const Vector3& position, const Quaternion& rotation, const Vector3& scale, bool_t isTrigger, bool_t isStatic, const
                                              float_t height,
                                              const float_t radius
    );
    [[nodiscard]]
    XNOR_ENGINE static uint32_t CreateConvexHull(Collider* c, const Vector3& position, const Quaternion& rotation, const Vector3& scale, const std::vector<Vertex>& vertices, bool_t isTrigger, bool_t isStatic);

    XNOR_ENGINE static void DestroyBody(uint32_t bodyId);

    [[nodiscard]]
    XNOR_ENGINE static bool_t IsBodyActive(uint32_t bodyId);
    [[nodiscard]]
    XNOR_ENGINE static Vector3 GetBodyPosition(uint32_t bodyId);
    [[nodiscard]]
    XNOR_ENGINE static Quaternion GetBodyRotation(uint32_t bodyId);

    XNOR_ENGINE static void SetPosition(uint32_t bodyId, const Vector3& position);
    XNOR_ENGINE static void SetRotation(uint32_t bodyId, const Quaternion& rotation);

    [[nodiscard]]
    XNOR_ENGINE static Collider* GetColliderFromId(uint32_t bodyId);

private:
    XNOR_ENGINE static void TraceImpl(const char_t* format, ...);

    [[nodiscard]]
    XNOR_ENGINE static uint32_t CreateBody(Collider* c, JPH::BodyCreationSettings& settings, bool_t isTrigger, bool_t isStatic);

    static inline std::unordered_map<uint32_t, Collider*> m_BodyMap;

    XNOR_ENGINE static inline JPH::PhysicsSystem* m_PhysicsSystem;
    XNOR_ENGINE static inline JPH::TempAllocatorImpl* m_Allocator;
    XNOR_ENGINE static inline JPH::JobSystemThreadPool* m_JobSystem;

    XNOR_ENGINE static inline BroadPhaseLayerInterfaceImpl m_BroadPhaseLayerInterface;
    XNOR_ENGINE static inline JPH::ObjectVsBroadPhaseLayerFilter m_ObjectVsBroadphaseLayerFilter;
    XNOR_ENGINE static inline JPH::ObjectLayerPairFilter m_ObjectVsObjectLayerFilter;
    XNOR_ENGINE static inline BodyActivationListenerImpl m_BodyActivationListener;
    XNOR_ENGINE static inline ContactListenerImpl m_ContactListener;

    XNOR_ENGINE static inline JPH::BodyInterface* m_BodyInterface;
};

END_XNOR_CORE
