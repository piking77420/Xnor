#pragma once

#include "render_window.hpp"

BEGIN_XNOR_EDITOR

class EditorWindow : public RenderWindow
{
    static constexpr float_t MaxPitch = 89.0f;
    
public:
    EditorWindow(Editor* editor);

    ~EditorWindow() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS_NO_ENGINE(EditorWindow)

    void Display() override;

    float_t lastX = 0.f;
    float_t lastY = 0.f;

private:
    XnorCore::Camera* m_EditorCamRef;
    
    bool m_FirstMove = false;
    float_t m_MouseSensitivity = 0.1f;
    float_t m_Yaw = 90.f;
    float_t m_Pitch = 0.f;
    bool m_IsMoving = false;

    void CameraControl();
    
    void OnGotoObject();
    
    void EditorCameraUpdate();

    void EditorCameraRotation();
    
    void EditorCameraMovement();

    void ResetCameraRotation();

    void EditCameraEditor();
};

END_XNOR_EDITOR
