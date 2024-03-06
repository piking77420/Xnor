#pragma once

#include "definitions.hpp"
#include "core.hpp"
#include "rendering/camera.hpp"

BEGIN_XNOR_EDITOR

class Editor;

class EditorCamera
{
    static constexpr float_t MaxPitch = 89.0f;

public:

    DEFAULT_COPY_MOVE_OPERATIONS(EditorCamera)

    EditorCamera(Editor& editor, XnorCore::Camera& camera);
    
    void UpdateCamera();
    
    void OnPressGoToObject();
private:
    Editor* m_EditorRef = nullptr;
    XnorCore::Camera* m_EditorRefCamera = nullptr;

    // Go to object variable
    Vector3 m_ObjectPos;
    float_t m_DistanceToStop = 0.f;
    
    Vector2 m_LastInput;
    Vector2 m_MouseOffSet;
    
    float_t m_MouseSensitivity = 0.1f;
    float_t m_Yaw = -90.f;
    float_t m_Pitch = 0.f;
    float_t m_CameraSpeed = 2.5f;

    bool_t m_IsMoving = false;
    bool_t m_FirstMove = false;
    bool_t m_ComputeDeltaMouse = false;
    bool_t m_ResetDeltaMouse = false;
    
    void ResetDeltatMouse();

    void CameraOnRightClick();
    
    void EditorCameraRotation();

    void EditorCameraMovement();

    void OnMiddleButton();

    void ComputeDeltaMouse();


    void GoToObject();

    void AddMovement(const Vector3& movement);
};

END_XNOR_EDITOR
