#pragma once

#include "render_window.hpp"

BEGIN_XNOR_EDITOR

class EditorWindow : public RenderWindow
{
private:
    static constexpr float_t MaxPitch = 89.0f;
    
public:
    EditorWindow(Editor* editor,const std::string& name, XnorCore::RendererContext* rendererContext);

    ~EditorWindow() override = default;

    void Display() override;

    float_t lastX = 0.f;
    float_t lastY = 0.f;

private:

    bool m_FirstMove = false;
    float_t m_MouseSensityvity = 0.1f;
    float_t m_Yaw = 0.f;
    float_t m_Pitch = 0.f;
    bool m_IsMoving = false;
    
    void EditorCameraUpdate();

    void EditorCameraRotation();
    
    void EditorCameraMovement();

    void ResetCameraRotation();
};
END_XNOR_EDITOR
