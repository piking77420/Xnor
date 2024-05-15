#include "scene/component/player_movement.hpp"

#include "input/input.hpp"
#include "input/time.hpp"
#include "scene/entity.hpp"

using namespace XnorCore;

void PlayerMovement::Update()
{
    Component::Update();
    Movement();
}

void PlayerMovement::Begin()
{
    Component::Begin();
    collider = entity->GetComponent<Collider>();
}

void PlayerMovement::Movement()
{
    float_t scalar = Time::GetDeltaTime() * speed;
    Vector3 vec;
     
    if (Input::GetKey(Key::W))
        vec += entity->transform.GetForward();

    if (Input::GetKey(Key::S))
        vec -= entity->transform.GetForward();
    
    if (Input::GetKey(Key::Q))
        vec += entity->transform.GetRight();

    if (Input::GetKey(Key::D))
        vec -= entity->transform.GetRight();
    
    collider->AddForce(vec * scalar);
    
}

void PlayerMovement::CameraMovement()
{
    Vector3 eulerRotation;

    
    
}
