#pragma once

#include "core.hpp"
#include "physics/layers.hpp"
#include "jolt/Jolt.h"
#include "Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h"
#include "jolt/Physics/Collision/ObjectLayer.h"

BEGIN_XNOR_CORE

/// @private
class XNOR_ENGINE BroadPhaseLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
{
public:
    BroadPhaseLayerInterfaceImpl();

    [[nodiscard]]
    JPH::uint GetNumBroadPhaseLayers() const override;

    [[nodiscard]]
    JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    virtual const char_t* GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override;
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

private:
    JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
};

END_XNOR_CORE
