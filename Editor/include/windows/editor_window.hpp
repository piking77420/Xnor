#pragma once

#include "editor_camera.hpp"
#include "render_window.hpp"
#include "editing/transform_gizmo.hpp"

BEGIN_XNOR_EDITOR
class EditorWindow : public RenderWindow
{
public:
    explicit EditorWindow(Editor* editor, XnorCore::Viewport& viewport);

    ~EditorWindow() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(EditorWindow)

    void Display() override;

    void GetEntitiesFromPixel(Vector2i pixel,XnorCore::Entity** ptr);

protected:
    void DrawOnTopOfImage();

    bool EditTransform();

    void SelectEntityOnScreen();
    
private:
    EditorCamera m_EditorCamera;
    TransfromGizmo m_TransfromGizmo;

    
};

END_XNOR_EDITOR
