#pragma once

#include "skinned_mesh_renderer.hpp"
#include "physics/components/collider.hpp"
#include "resource/animation_montage.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE

/// @private
class TestComponentAnimation final : public Component
{
    REFLECTABLE_IMPL(TestComponentAnimation)

public:
    DEFAULT_COPY_MOVE_OPERATIONS(TestComponentAnimation)
    
    XNOR_ENGINE void Begin() override;

    XNOR_ENGINE void Update() override;

    XNOR_ENGINE TestComponentAnimation() = default;
    XNOR_ENGINE ~TestComponentAnimation() override = default;

private:
    SkinnedMeshRenderer* m_Renderer;
    AnimationMontage m_Montage;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::TestComponentAnimation, bases<XnorCore::Component>));
