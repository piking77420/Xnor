#include "scene/component/player_shoot_cpp.hpp"

#include "input/input.hpp"
#include "physics/component/sphere_collider.hpp"
#include "resource/resource_manager.hpp"
#include "world/world.hpp"

using namespace XnorCore;

void SetupBullet(Entity* const bullet)
{
    StaticMeshRenderer* const meshRenderer = bullet->AddComponent<StaticMeshRenderer>();
    meshRenderer->mesh = ResourceManager::Get<Mesh>("assets/models/sphere.obj");
    meshRenderer->material.emissiveColor = Color::Green();
    meshRenderer->material.emissive = 1000.f;
}

void PlayerShootCpp::Shoot()
{
    Entity* const bullet = World::scene->CreateEntity("Bullet");
    SetupBullet(bullet);

    SphereCollider* coll = bullet->AddComponent<SphereCollider>();
    coll->SetRadius(bulletSize);

    AudioSource* const audioSource = bullet->AddComponent<AudioSource>();
    audioSource->audioTrack = ResourceManager::Get<AudioTrack>("assets/sounds/blaster.wav");
    audioSource->SetLooping(false);
    audioSource->Play();
            
    Matrix shootPointMatrix = shootPoint->transform.worldMatrix;
    bullet->transform.SetPosition( {shootPointMatrix.m03, shootPointMatrix.m13, shootPointMatrix.m23}) ;
    bullet->transform.SetScale( bullet->transform.GetScale() * bulletSize);
    const Vector3 force = (bullet->transform.GetPosition() - parent->transform.GetPosition()).Normalized() * 10000.f;
    coll->AddForce(force);
}

void PlayerShootCpp::BulletSound()
{
}

void PlayerShootCpp::Awake()
{
}

void PlayerShootCpp::Begin()
{
    parent = entity->GetParent();
    shootPoint = entity->GetChild(0);
}

void PlayerShootCpp::Update()
{
    if (Input::GetMouseButton(MouseButton::Right, MouseButtonStatus::Pressed))
        Shoot();
}
