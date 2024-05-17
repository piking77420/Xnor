#include "scene/component/camera_component.hpp"

#include "application.hpp"

using namespace  XnorCore;

CameraComponent::CameraComponent()
{
    Application::applicationInstance->gameViewPort->camera = &this->camera;
}

CameraComponent::~CameraComponent()
{
    if (Application::applicationInstance->gameViewPort->camera == &this->camera)
        Application::applicationInstance->gameViewPort->camera = nullptr;
}

void CameraComponent::Update()
{
    Component::Update();
    // FIXME: This causes clipping problems
    /*const Vector3 pos = static_cast<Vector3>(entity->transform.worldMatrix[3]);
    camera.position = pos;
    camera.LookAt(pos + entity->transform.GetForward(), entity->transform.GetUp());*/
}

void CameraComponent::Begin()
{
    Component::Begin();
}
