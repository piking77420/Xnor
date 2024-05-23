#pragma once
#include "core.hpp"
#include "skinned_mesh_renderer.hpp"
#include "physics/component/capsule_collider.hpp"
#include "resource/animation.hpp"
#include "scene/component.hpp"
#include "utils/coroutine.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_CORE
    struct CollisionData;
    class Collider;

class EnemyCpp : public Component
{
    REFLECTABLE_IMPL(EnemyCpp)
public:
    DEFAULT_COPY_MOVE_OPERATIONS(EnemyCpp)

    XNOR_ENGINE EnemyCpp() = default;

    XNOR_ENGINE ~EnemyCpp() = default;

    XNOR_ENGINE void Awake() override;
    
    XNOR_ENGINE void Update() override;
    
    XNOR_ENGINE void OnRendering() override;
    
private:
    Pointer<Animation> m_Idle;

    Pointer<Animation> m_Run;

    Pointer<Animation> m_Attack;

    SkinnedMeshRenderer* skinnedMeshRenderer = nullptr;

    bool_t m_IsInDetectionRange;

    const Entity* player;

    CapsuleCollider* capsule = nullptr;

    float_t m_MoveSpeed = 1.f;

    float_t KillY = -1000.f;

    float_t m_AttackRange = 1.f;

    bool_t m_IsAttacking = false;
    
    
    XNOR_ENGINE void OnDetectionEnter(Collider* coll1, Collider* coll2, const CollisionData& data);

    XNOR_ENGINE void OnDetectionExit(Collider* coll1, Collider* coll2);
    
    XNOR_ENGINE void OnTriggerStay(Collider* coll1, Collider* coll2, const CollisionData& data);
    
    XNOR_ENGINE void Move();

    XNOR_ENGINE bool_t IsInRange() const;

    XNOR_ENGINE void Attack();

    XNOR_ENGINE Coroutine ResetDirtyFlagAttackRoutine();

};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::EnemyCpp, bases<XnorCore::Component>),
    field(m_Idle),
    field(m_Run),
    field(m_Attack),
    field(m_MoveSpeed),
    field(m_AttackRange)
);
