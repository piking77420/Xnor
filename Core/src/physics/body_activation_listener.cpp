#include "physics/body_activation_listener.hpp"
#include "jolt/Physics/Body/BodyID.h"

#include "utils/logger.hpp"

using namespace XnorCore;

void BodyActivationListenerImpl::OnBodyActivated(const JPH::BodyID& inBodyId, const JPH::uint64)
{
    Logger::LogInfo("Body {} activated", inBodyId.GetIndexAndSequenceNumber());
}

void BodyActivationListenerImpl::OnBodyDeactivated(const JPH::BodyID& inBodyId, const JPH::uint64)
{
    Logger::LogInfo("Body {} de-activated", inBodyId.GetIndexAndSequenceNumber());
}
