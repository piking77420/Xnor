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
    {
        Application::applicationInstance->gameViewPort->camera = nullptr;
    }
}

void CameraComponent::Update()
{
    const Transform& transform = GetTransform();
    Component::Update();
    camera.position = static_cast<Vector3>(transform.worldMatrix[3]);
    camera.LookAt(Matrix3(transform.worldMatrix) * (camera.position + Vector3::UnitZ()));
}

void CameraComponent::Begin()
{
    Component::Begin();
}
