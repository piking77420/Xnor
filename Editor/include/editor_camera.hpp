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
    void ResetDeltatMouse();

    void CameraOnRightClick(XnorCore::Camera* camera);
    
    void EditorCameraRotation(XnorCore::Camera* camera);

    void EditorCameraMovement(XnorCore::Camera* camera);

    void OnMiddleButton(XnorCore::Camera* camera);

    void ComputeDeltaMouse();
    
    bool m_FirstMove = false;
    float_t m_MouseSensitivity = 0.1f;
    float_t m_Yaw = 90.f;
    float_t m_Pitch = 0.f;
    Vector2 m_LastInput;
    bool m_IsMoving = false;
    float_t m_CameraSpeed = 2.5f;

    bool m_ComputeDeltaMouse = false;
    bool m_ResetDeltaMouse = false;

    Vector2 m_MouseOffSet;
    
};

END_XNOR_EDITOR