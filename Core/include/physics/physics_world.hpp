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
    struct BodyCreationInfo
    {
        Collider* collider = nullptr;
        
        Vector3 position;
        Quaternion rotation;
        Vector3 scaling;

        bool_t isTrigger{};
        bool_t isStatic{};
    };

    struct RaycastResult
    {
        Collider* hitBody = nullptr;
        Vector3 point;
        Vector3 normal;
        float_t distance{};
    };
    
    XNOR_ENGINE static void Initialize();
    XNOR_ENGINE static void Destroy();
    XNOR_ENGINE static void Update(float_t deltaTime);

    XNOR_ENGINE static void SetGravity(const Vector3& gravity);

    [[nodiscard]]
    XNOR_ENGINE static uint32_t CreateSphere(const BodyCreationInfo& info, float_t radius);
    [[nodiscard]]
    XNOR_ENGINE static uint32_t CreateBox(const BodyCreationInfo& info);
    [[nodiscard]]
    XNOR_ENGINE static uint32_t CreateCapsule(const BodyCreationInfo& info, float_t height, float_t radius);
    [[nodiscard]]
    XNOR_ENGINE static uint32_t CreateConvexHull(const BodyCreationInfo& info, const std::vector<Vertex>& vertices);

    XNOR_ENGINE static void DestroyBody(uint32_t bodyId);

    [[nodiscard]]
    XNOR_ENGINE static bool_t IsBodyActive(uint32_t bodyId);
    [[nodiscard]]
    XNOR_ENGINE static Vector3 GetBodyPosition(uint32_t bodyId);
    [[nodiscard]]
    XNOR_ENGINE static Quaternion GetBodyRotation(uint32_t bodyId);

    XNOR_ENGINE static void SetPosition(uint32_t bodyId, const Vector3& position);
    XNOR_ENGINE static void SetRotation(uint32_t bodyId, const Quaternion& rotation);

    XNOR_ENGINE static void AddForce(uint32_t bodyId, const Vector3& force);
    XNOR_ENGINE static void AddForce(uint32_t bodyId, const Vector3& force, const Vector3& point);
    XNOR_ENGINE static void AddImpulse(uint32_t bodyId, const Vector3& impulse);
    XNOR_ENGINE static void AddImpulse(uint32_t bodyId, const Vector3& impulse, const Vector3& point);
    XNOR_ENGINE static void AddTorque(uint32_t bodyId, const Vector3& torque);

    [[nodiscard]]
    XNOR_ENGINE static Collider* GetColliderFromId(uint32_t bodyId);

    XNOR_ENGINE static bool_t Raycast(const Vector3& position, const Vector3& direction, float_t length, RaycastResult& result);

private:
    XNOR_ENGINE static void TraceImpl(const char_t* format, ...);

    [[nodiscard]]
    XNOR_ENGINE static uint32_t CreateBody(const BodyCreationInfo& info, JPH::BodyCreationSettings& settings);

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
