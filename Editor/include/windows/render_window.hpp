#pragma once

#include "definitions.hpp"
#include "ui_window.hpp"
#include "rendering/camera.hpp"
#include "rendering/frame_buffer.hpp"

BEGIN_XNOR_EDITOR

class RenderWindow : public UiWindow
{
public:
    RenderWindow(Editor* editor, const std::string& name, XnorCore::RendererContext* rendererContext);

    virtual ~RenderWindow() override;

    void Display() override;

protected:
    XnorCore::Camera m_Camera;
    
    XnorCore::FrameBuffer m_FrameBuffer;
    
    XnorCore::RenderPass m_RenderPass;
    
    XnorCore::Texture* m_ColorTexture = nullptr;
    
    XnorCore::Texture* m_DepthTexture = nullptr;
};

END_XNOR_EDITOR
