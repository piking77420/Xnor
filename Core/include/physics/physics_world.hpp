#pragma once

#include "core.hpp"

#include "Jolt/Physics/PhysicsSystem.h"
#include "Jolt/Core/JobSystemThreadPool.h"

BEGIN_XNOR_CORE

class XNOR_ENGINE PhysicsWorld
{
    STATIC_CLASS(PhysicsWorld)

public:
    static void Initialize();
    static void Update(float_t deltaTime);

private:
    static void TraceImpl(const char_t* format, ...);

    static inline JPH::PhysicsSystem m_PhysicsSystem;
    static inline JPH::TempAllocatorImpl m_Allocator;
    static inline JPH::JobSystemThreadPool m_JobSystem;
};

END_XNOR_CORE
