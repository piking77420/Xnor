#include "scene/component/player_shoot_cpp.hpp"

#include "input/input.hpp"
#include "physics/component/sphere_collider.hpp"
#include "resource/resource_manager.hpp"
#include "world/world.hpp"

using namespace XnorCore;


void SetupBullet(Entity* bullet)
{
    StaticMeshRenderer* meshRenderer = bullet->AddComponent<StaticMeshRenderer>();
    meshRenderer->mesh = ResourceManager::Get<Mesh>("assets/models/sphere.obj");
    meshRenderer->material.emissiveColor = Color::Green();
    meshRenderer->material.emissive = 1000.f;
}

void PlayerShootCpp::Shoot()
{
            
    Entity* bullet = World::scene->CreateEntity("Bullet");
    SetupBullet(bullet);
    SphereCollider* coll = bullet->AddComponent<SphereCollider>();
    coll->SetRadius(bulletSize);
    auto s = bullet->AddComponent<AudioSource>();
    s->audioTrack = ResourceManager::Get<AudioTrack>("assets/sounds/blaster.wav");
    s->SetLooping(false);
    s->Play();
            
    Matrix shootPointMatrix = shootPoint->transform.worldMatrix;
    bullet->transform.SetPosition( {shootPointMatrix.m03, shootPointMatrix.m13, shootPointMatrix.m23}) ;
    bullet->transform.SetScale( bullet->transform.GetScale() * bulletSize);
    Vector3 force = (bullet->transform.GetPosition() - parent->transform.GetPosition()).Normalized() * 10000.f;
    coll->AddForce(force);
}

void PlayerShootCpp::BulletSound()
{
    
}

void XnorCore::PlayerShootCpp::Awake()
{
}

void XnorCore::PlayerShootCpp::Begin()
{
    parent = entity->GetParent();
    shootPoint = entity->GetChild(0);
}

void XnorCore::PlayerShootCpp::Update()
{
    if (Input::GetMouseButton(MouseButton::Right, MouseButtonStatus::Pressed))
        Shoot();
    
}
