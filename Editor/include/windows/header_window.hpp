#pragma once

#include "definitions.hpp"
#include "ui_window.hpp"
#include "resource/texture.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_EDITOR
    class HeaderWindow : public UiWindow
{
private:
    static constexpr float_t m_TileBaseSize = 25.f; 
    
public:
    HeaderWindow();
    ~HeaderWindow() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS_NO_ENGINE(HeaderWindow)
    
    void Display() override;

private:

    void DisplayOnEditor();
    void DisplayOnPlay();

    Vector2 m_ImagePos[2];
    float_t m_ImageSize = 1.f;
    
    
    XnorCore::Pointer<XnorCore::File> PauseButtonPng;
    XnorCore::Pointer<XnorCore::Texture> PauseButton;

    XnorCore::Pointer<XnorCore::File> PlayButtonPng;
    XnorCore::Pointer<XnorCore::Texture> PlayButton;

    
    XnorCore::Pointer<XnorCore::File> StopButtonPng;
    XnorCore::Pointer<XnorCore::Texture> StopButton;
};

END_XNOR_EDITOR
