#pragma once

#include "definitions.hpp"
#include "ui_window.hpp"
#include "rendering/camera.hpp"
#include "rendering/frame_buffer.hpp"

BEGIN_XNOR_EDITOR

class RenderWindow : public UiWindow
{
public:

    RenderWindow(Editor* editor);
    
    RenderWindow(Editor* editor, const std::string& title);

    ~RenderWindow() override;

    void Display() override;

protected:
    XnorCore::RendererContext m_RendererContext;
    
    XnorCore::FrameBuffer m_FrameBuffer;
    
    XnorCore::RenderPass m_RenderPass;
    
    XnorCore::Texture* m_ColorTexture = nullptr;
private:
    void Initialize(XnorCore::RendererContext* rendererContext);
};

END_XNOR_EDITOR
