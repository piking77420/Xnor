#pragma once

#include "definitions.hpp"
#include "ui_window.hpp"
#include "rendering/camera.hpp"
#include "rendering/frame_buffer.hpp"

BEGIN_XNOR_EDITOR

class RenderWindow : public UiWindow
{
public:
    RenderWindow(Editor* editor, XnorCore::RendererContext* rendererContext);
    
    RenderWindow(Editor* editor, XnorCore::RendererContext* rendererContext, const std::string& title);

    ~RenderWindow() override;

    void Display() override;

protected:
    XnorCore::Camera m_Camera;
    
    XnorCore::FrameBuffer m_FrameBuffer;
    
    XnorCore::RenderPass m_RenderPass;
    
    XnorCore::Texture* m_ColorTexture = nullptr;
    
    XnorCore::Texture* m_DepthTexture = nullptr;

private:
    void Initialize(XnorCore::RendererContext* rendererContext);
};


END_XNOR_EDITOR
