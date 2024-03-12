#pragma once

#include "core.hpp"
#include "scene/component.hpp"
#include "physics/components/collider.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE BoxCollider : public Collider
{
    REFLECTABLE_IMPL_MINIMAL(BoxCollider)

public:
    BoxCollider() = default; 
    ~BoxCollider() override = default;

    void Begin() override;
    void Update() override;
    
    DEFAULT_COPY_MOVE_OPERATIONS(BoxCollider)
    
private:
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::BoxCollider, bases<XnorCore::Collider>)
)
