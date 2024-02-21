#pragma once

#include "definitions.hpp"
#include "ui_window.hpp"
#include "rendering/camera.hpp"
#include "rendering/frame_buffer.hpp"

BEGIN_XNOR_EDITOR

class RenderWindow : public UiWindow
{
public:
<<<<<<< HEAD
    RenderWindow(Editor* editor, const std::string& name, XnorCore::RendererContext* rendererContext);
=======
    RenderWindow(Editor* editor, XnorCore::RendererContext* rendererContext);
    
    RenderWindow(Editor* editor, XnorCore::RendererContext* rendererContext, const std::string& title);
>>>>>>> ea87a0e837c38a39fdd2c904941d61d2a9806628

    ~RenderWindow() override;

    void Display() override;

protected:
    XnorCore::Camera m_Camera;
    
    XnorCore::FrameBuffer m_FrameBuffer;
    
    XnorCore::RenderPass m_RenderPass;
    
    XnorCore::Texture* m_ColorTexture = nullptr;
    
    XnorCore::Texture* m_DepthTexture = nullptr;
<<<<<<< HEAD
=======

private:
    void Initialize(XnorCore::RendererContext* rendererContext);
>>>>>>> ea87a0e837c38a39fdd2c904941d61d2a9806628
};

END_XNOR_EDITOR
