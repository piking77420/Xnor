#pragma once

#include "definitions.hpp"
#include "ui_window.hpp"
#include "rendering/camera.hpp"
#include "rendering/frame_buffer.hpp"

BEGIN_XNOR_EDITOR

class RenderWindow : public UiWindow
{
public:
    
    RenderWindow(Editor* editor,XnorCore::RendererContext* rendererContext);

    ~RenderWindow() override;

    void Display() override;

private:

    std::string m_DisplayName;
    
    XnorCore::Camera m_Camera;
    XnorCore::FrameBuffer m_FrameBuffer;
    XnorCore::RenderPass m_RenderPass;
    XnorCore::Texture* m_ColorTexture = nullptr;
    XnorCore::Texture* m_DepthTexture = nullptr;

};


END_XNOR_EDITOR
