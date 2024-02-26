#pragma once

#include "editor_camera.hpp"
#include "render_window.hpp"

BEGIN_XNOR_EDITOR

class EditorWindow : public RenderWindow
{
public:
    EditorWindow(Editor* editor);

    ~EditorWindow() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS_NO_ENGINE(EditorWindow)

    void Display() override;
private:
    EditorCamera m_EditorCamera;
};

END_XNOR_EDITOR
