#pragma once

#include "definitions.hpp"
#include "ui_window.hpp"
#include "rendering/camera.hpp"
#include "rendering/frame_buffer.hpp"

BEGIN_XNOR_EDITOR

class RenderWindow : public UiWindow
{
public:
    explicit RenderWindow(Editor* editor);
    
    RenderWindow(Editor* editor, const std::string& title);

    ~RenderWindow() override = default;
    
    DEFAULT_COPY_MOVE_OPERATIONS(RenderWindow);
    
    void Display() override;

    void OnApplicationRendering() override;

protected:
    XnorCore::Viewport m_Vieport;
};

END_XNOR_EDITOR
