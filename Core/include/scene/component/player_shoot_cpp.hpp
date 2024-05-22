#pragma once
#include "core.hpp"
#include "audio/component/audio_listener.hpp"
#include "audio/component/audio_source.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE
class PlayerShootCpp : public Component
{
    REFLECTABLE_IMPL(PlayerShootCpp)
public:
    DEFAULT_COPY_MOVE_OPERATIONS(PlayerShootCpp)

    PlayerShootCpp() = default;

    ~PlayerShootCpp() = default;

    void Awake() override;
    
    void Begin() override;
    
    void Update() override;
    
    
private:
    Entity* parent = nullptr;
    Entity* shootPoint = nullptr;
    
    float bulletSize = 0.2f;

    void Shoot();

    void BulletSound();
};

END_XNOR_CORE
/// @private
REFL_AUTO(
    type(XnorCore::PlayerShootCpp, bases<XnorCore::Component>)
);
