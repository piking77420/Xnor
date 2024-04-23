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
    Component::Update();
}

void CameraComponent::Begin()
{
    Component::Begin();
}
