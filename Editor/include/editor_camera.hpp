#pragma once

#include "definitions.hpp"
#include "core.hpp"
#include "rendering/camera.hpp"

BEGIN_XNOR_EDITOR
class EditorCamera
{
private:
    static constexpr float_t MaxPitch = 89.0f;

public:
    void UpdateCamera(XnorCore::Camera* camera);

private:
    void ResetCameraRotation(XnorCore::Camera* camera);
    
    void EditorCameraRotation(XnorCore::Camera* camera);

    void EditorCameraMovement(XnorCore::Camera* camera);
    
    bool m_FirstMove = false;
    float_t m_MouseSensitivity = 0.1f;
    float_t m_Yaw = 90.f;
    float_t m_Pitch = 0.f;
    float_t m_LastX = 0.f;
    float_t m_LastY = 0.f;
    bool m_IsMoving = false;
    float_t m_CameraSpeed = 2.5f;
    
};

END_XNOR_EDITOR