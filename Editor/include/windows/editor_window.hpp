#pragma once

#include "render_window.hpp"

BEGIN_XNOR_EDITOR

class EditorWindow : public RenderWindow
{
public:
    EditorWindow(Editor* editor,const std::string& name, XnorCore::RendererContext* rendererContext);

    ~EditorWindow() override = default;

    void Display() override;

private:
    void EditorCameraUpdate();
};
END_XNOR_EDITOR
