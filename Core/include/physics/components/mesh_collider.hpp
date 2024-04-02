#pragma once

#include "core.hpp"
#include "scene/component.hpp"
#include "physics/components/collider.hpp"

BEGIN_XNOR_CORE

class MeshCollider : public Collider
{
    REFLECTABLE_IMPL(MeshCollider)

public:
    XNOR_ENGINE MeshCollider() = default; 
    XNOR_ENGINE ~MeshCollider() override = default;

    XNOR_ENGINE void Begin() override;
    XNOR_ENGINE void Update() override;
    
    DEFAULT_COPY_MOVE_OPERATIONS(MeshCollider)
    
private:
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::MeshCollider, bases<XnorCore::Collider>)
)
