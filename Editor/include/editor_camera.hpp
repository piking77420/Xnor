#pragma once

#include "definitions.hpp"
#include "core.hpp"
#include "rendering/camera.hpp"

BEGIN_XNOR_EDITOR
class Editor;

class EditorCamera
{
private:
    static constexpr float_t MaxPitch = 89.0f;

public:
    void UpdateCamera(const XnorEditor::Editor& editor,XnorCore::Camera& camera);
private:
    const Editor* m_EditorRef = nullptr;
    XnorCore::Camera* m_EditorRefCamera = nullptr;

    // Go to object variable
    Vector3 m_ObjectPos;
    float_t m_DistanceToStop = 0.f;
    
    Vector2 m_LastInput;
    Vector2 m_MouseOffSet;
    
    float_t m_MouseSensitivity = 0.1f;
    float_t m_Yaw = 90.f;
    float_t m_Pitch = 0.f;
    float_t m_CameraSpeed = 2.5f;

    bool m_IsMoving = false;
    bool m_FirstMove = false;
    bool m_ComputeDeltaMouse = false;
    bool m_ResetDeltaMouse = false;
    bool m_GotoObject = false;
    
    void ResetDeltatMouse();

    void CameraOnRightClick();
    
    void EditorCameraRotation();

    void EditorCameraMovement();

    void OnMiddleButton();

    void ComputeDeltaMouse();

    void OnPressGoToObject();

    void GoToObject();
};


END_XNOR_EDITOR
