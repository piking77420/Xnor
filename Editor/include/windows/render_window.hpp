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

    ~RenderWindow() override;
    
    DEFAULT_COPY_MOVE_OPERATIONS_NO_ENGINE(RenderWindow);
    
    void Display() override;

    void OnWindowResize(Vector2i newWindowSize) override;

protected:
    XnorCore::RendererContext m_RendererContext;
    
    XnorCore::FrameBuffer m_FrameBuffer;
    
    XnorCore::RenderPass m_RenderPass;
    
    XnorCore::Texture* m_ColorTexture = nullptr;

    void ResizeRenderContext(Vector2i size);
    
    virtual void Initialize(Vector2i size);
};

END_XNOR_EDITOR
