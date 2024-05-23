﻿#include "scene/component/enemy_cpp.hpp"

#include "Jolt/Core/Core.h"
#include "physics/component/sphere_collider.hpp"
#include "scene/entity.hpp"
#include "world/world.hpp"

using namespace XnorCore;

void EnemyCpp::OnDetectionEnter(Collider* coll1, Collider* coll2, const CollisionData&)
{
    if (coll2->entity->name != "Player")
        return;

    Logger::LogInfo("Maplhite will follow this entity = {}",coll2->entity->name);
    skinnedMeshRenderer->StartAnimation(m_Run);
    player = coll2->entity;
    m_IsInDetectionRange = true;
}

void EnemyCpp::OnDetectionExit(Collider* coll1, Collider* coll2)
{
    if (coll2->entity->name != "Player")
        return;

    Logger::LogInfo("Maplhite Lost interest = {}",coll2->entity->name);
    skinnedMeshRenderer->StartAnimation(m_Idle);
    player = coll2->entity;
    m_IsInDetectionRange = false;
    capsule->SetLinearVelocity(Vector3::Zero());
}

void EnemyCpp::OnTriggerStay(Collider* coll1, Collider* coll2, const CollisionData& data)
{
    if (coll2->entity->name != "Player")
        return;

    if (m_IsAttacking)
        return;
    
    if (IsInRange())
    {
        Attack();
    }
    else
    {
        Move();
    }
}

void EnemyCpp::Move()
{
    if (entity == nullptr)
        return;
    
    const Vector3 pos1 = GetTransform().GetPosition();
    const Vector3 pos2 = player->transform.GetPosition();

    const Vector3 fow = (pos2 - pos1).Normalized();

    Vector3 fowLook = fow;
    fowLook.y = 0.f; 

    const Vector3 targetPosition = pos1 + fowLook;

    entity->LookAt(pos1, targetPosition);

    // Update velocity
    const Vector3 current_velocity = capsule->GetLinearVelocity();
    Vector3 desired_velocity = fow * m_MoveSpeed;
    desired_velocity.y = current_velocity.y;
    const Vector3 new_velocity = 0.75f * current_velocity + 0.25f * desired_velocity;
            

    // Update position
    capsule->SetLinearVelocity(new_velocity);
}

bool_t EnemyCpp::IsInRange() const
{
    return (entity->transform.GetPosition() - player->transform.GetPosition()).Length() <= m_AttackRange;
}

void EnemyCpp::Attack()
{
    skinnedMeshRenderer->StartAnimation(m_Attack);
    //m_IsAttacking = true;
    //Coroutine::Start(ResetDirtyFlagAttackRoutine());
}

Coroutine EnemyCpp::ResetDirtyFlagAttackRoutine()
{
    using namespace std::chrono_literals;
    co_await 2s;
    m_IsAttacking = false;
}


void EnemyCpp::Awake()
{
    Component::Awake();
    SphereCollider* s = entity->GetComponent<SphereCollider>();
    capsule = entity->GetComponent<CapsuleCollider>();
    
    s->onTriggerEnter += [this](Collider* coll1, Collider* coll2, const CollisionData& data) { OnDetectionEnter(coll1, coll2, data); };
    s->onTriggerExit += [this](Collider* coll1, Collider* coll2) { OnDetectionExit(coll1, coll2); };
    s->onTriggerStay += [this](Collider* coll1, Collider* coll2 ,const CollisionData& data) { OnTriggerStay(coll1, coll2, data); };
    skinnedMeshRenderer = entity->GetComponent<SkinnedMeshRenderer>();
    skinnedMeshRenderer->StartAnimation(m_Idle);
}

void EnemyCpp::Update()
{
    Component::Update();

  
}

void EnemyCpp::OnRendering()
{
    Component::OnRendering();
    DrawGizmo::Sphere(entity->transform.GetPosition() ,m_AttackRange, Color::Red());
}
