#pragma once

#include "editor_camera.hpp"
#include "render_window.hpp"
#include "editing/transform_gizmo.hpp"

BEGIN_XNOR_EDITOR
    class EditorWindow : public RenderWindow
{
public:
    explicit EditorWindow(Editor* editor);

    ~EditorWindow() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(EditorWindow)

    void Display() override;

protected:
    void Initialize(Vector2i size) override;

    void DrawOnTopOfImage();

    void EditTransform();
    
private:
    EditorCamera m_EditorCamera;
    TransfromGizmo m_TransfromGizmo;

    
};

END_XNOR_EDITOR
