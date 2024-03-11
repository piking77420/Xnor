#include "physics/physics_world.hpp"

#include <cstdarg>
#include <Jolt/Jolt.h>

#include "Jolt/RegisterTypes.h"
#include "Jolt/Core/Factory.h"
#include "Jolt/Core/TempAllocator.h"
#include "Jolt/Physics/Collision/Shape/BoxShape.h"
#include "utils/logger.hpp"
#include "Jolt/Physics/Collision/Shape/SphereShape.h"
#include "jolt/Physics/Body/BodyCreationSettings.h"
#include "physics/body_activation_listener.hpp"
#include "physics/broad_phase_layer_interface.hpp"
#include "physics/contact_listener.hpp"

using namespace XnorCore;

using namespace JPH::literals;

namespace Layers
{
    static constexpr JPH::ObjectLayer NON_MOVING = 0;
    static constexpr JPH::ObjectLayer MOVING = 1;
    static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
};

void PhysicsWorld::Initialize()
{
    // Register allocation hook
    JPH::RegisterDefaultAllocator();

    // Install callbacks
    JPH::Trace = TraceImpl;

    // Create a factory
    JPH::Factory::sInstance = new JPH::Factory();

    // Register all Jolt physics types
    JPH::RegisterTypes();

    // We need a temp allocator for temporary allocations during the physics update. We're
    // pre-allocating 10 MB to avoid having to do allocations during the physics update.
    // B.t.w. 10 MB is way too much for this example but it is a typical value you can use.
    // If you don't want to pre-allocate you can also use TempAllocatorMalloc to fall back to
    // malloc / free.
    // TODO m_Allocator = JPH::TempAllocatorImpl(10 * 1024 * 1024);;
    
    // We need a job system that will execute physics jobs on multiple threads. Typically
    // you would implement the JobSystem interface yourself and let Jolt Physics run on top
    // of your own job scheduler. JobSystemThreadPool is an example implementation.
    // TODO m_JobSystem = JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, static_cast<int32_t>(std::thread::hardware_concurrency()) - 1);

    // This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
    // Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
    constexpr JPH::uint maxBodies = 1024;

    // This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
    constexpr JPH::uint numBodyMutexes = 0;

    // This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
    // body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
    // too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
    // Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
    constexpr JPH::uint maxBodyPairs = 1024;

    // This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
    // number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
    // Note: This value is low because this is a simple test. For a real project use something in the order of 10240.
    constexpr JPH::uint maxContactConstraints = 1024;

    // Create mapping table from object layer to broadphase layer
    // Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
    BroadPhaseLayerInterfaceImpl broadPhaseLayerInterface;

    // Create class that filters object vs broadphase layers
    // Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
    JPH::ObjectVsBroadPhaseLayerFilter objectVsBroadphaseLayerFilter;

    // Create class that filters object vs object layers
    // Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
    JPH::ObjectLayerPairFilter objectVsObjectLayerFilter;

    // Now we can create the actual physics system.
    m_PhysicsSystem.Init(maxBodies, numBodyMutexes, maxBodyPairs, maxContactConstraints, broadPhaseLayerInterface, objectVsBroadphaseLayerFilter, objectVsObjectLayerFilter);

    // A body activation listener gets notified when bodies activate and go to sleep
    // Note that this is called from a job so whatever you do here needs to be thread safe.
    // Registering one is entirely optional.
    BodyActivationListenerImpl bodyActivationListener;
    m_PhysicsSystem.SetBodyActivationListener(&bodyActivationListener);

    // A contact listener gets notified when bodies (are about to) collide, and when they separate again.
    // Note that this is called from a job so whatever you do here needs to be thread safe.
    // Registering one is entirely optional.
    ContactListenerImpl contactListener;
    m_PhysicsSystem.SetContactListener(&contactListener);

    // The main way to interact with the bodies in the physics system is through the body interface. There is a locking and a non-locking
    // variant of this. We're going to use the locking version (even though we're not planning to access bodies from multiple threads)
    JPH::BodyInterface& bodyInterface = m_PhysicsSystem.GetBodyInterface();

    // Next we can create a rigid body to serve as the floor, we make a large box
    // Create the settings for the collision volume (the shape).
    // Note that for simple shapes (like boxes) you can also directly construct a BoxShape.
    JPH::BoxShapeSettings floorShapeSettings(JPH::Vec3(100.0f, 1.0f, 100.0f));

    // Create the shape
    JPH::ShapeSettings::ShapeResult floorShapeResult = floorShapeSettings.Create();
    JPH::ShapeRefC floorShape = floorShapeResult.Get(); // We don't expect an error here, but you can check floor_shape_result for HasError() / GetError()

    // Create the settings for the body itself. Note that here you can also set other properties like the restitution / friction.
    JPH::BodyCreationSettings floorSettings(floorShape, JPH::RVec3(0.0_r, -1.0_r, 0.0_r), JPH::Quat::sIdentity(), JPH::EMotionType::Static, Layers::NON_MOVING);

    // Create the actual rigid body
    JPH::Body* floor = bodyInterface.CreateBody(floorSettings); // Note that if we run out of bodies this can return nullptr

    // Add it to the world
    bodyInterface.AddBody(floor->GetID(), JPH::EActivation::DontActivate);

    // Now create a dynamic body to bounce on the floor
    // Note that this uses the shorthand version of creating and adding a body to the world
    JPH::BodyCreationSettings sphereSettings(new JPH::SphereShape(0.5f), JPH::RVec3(0.0_r, 2.0_r, 0.0_r), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, Layers::MOVING);
    JPH::BodyID sphereId = bodyInterface.CreateAndAddBody(sphereSettings, JPH::EActivation::Activate);

    // Now you can interact with the dynamic body, in this case we're going to give it a velocity.
    // (note that if we had used CreateBody then we could have set the velocity straight on the body before adding it to the physics system)
    bodyInterface.SetLinearVelocity(sphereId, JPH::Vec3(0.0f, -5.0f, 0.0f));

    // Optional step: Before starting the physics simulation you can optimize the broad phase. This improves collision detection performance (it's pointless here because we only have 2 bodies).
    // You should definitely not call this every frame or when e.g. streaming in a new level section as it is an expensive operation.
    // Instead insert all new objects in batches instead of 1 at a time to keep the broad phase efficient.
    m_PhysicsSystem.OptimizeBroadPhase();
}

void PhysicsWorld::Update(const float_t deltaTime)
{
    m_PhysicsSystem.Update(deltaTime, 1, &m_Allocator, &m_JobSystem);
}

void PhysicsWorld::TraceImpl(const char_t* format, ...)
{
    // Format the message
    va_list list;
    va_start(list, format);
    char buffer[1024];
    (void)vsnprintf(buffer, sizeof(buffer), format, list);  // NOLINT(clang-diagnostic-format-nonliteral)
    va_end(list);

    // Print to the TTY
    const char_t* const buf = buffer;
    Logger::LogInfo("{}", buf);
}
