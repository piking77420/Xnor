#include "windows/render_window.hpp"

#include <ImGui/imgui_internal.h>

#include "world/world.hpp"


using namespace XnorEditor;

RenderWindow::RenderWindow(Editor* editor)
    : UiWindow(editor, "Game Preview")
{
    Initialize(XnorCore::Window::GetSize());
}

RenderWindow::RenderWindow(Editor* editor, const std::string& title)
    : UiWindow(editor, title)
{
    
}

void RenderWindow::ResizeRenderContext(const Vector2i size)
{
    delete m_ColorTexture;
    m_FrameBuffer.~FrameBuffer();
    Initialize(size);
}

RenderWindow::~RenderWindow()
{
    delete m_ColorTexture;
}

void RenderWindow::Display()
{
    if (!m_RendererContext.IsValid() || m_RendererContext.framebuffer->GetSize() != m_Size)
    {
        ResizeRenderContext(m_Size);
    }
    
    m_Editor->renderer.RenderScene(m_RendererContext);
    ImGui::Image(XnorCore::Utils::IntToPointer<ImTextureID>(m_ColorTexture->GetId()), ImGui::GetContentRegionAvail(),  ImVec2(0, 1), ImVec2(1, 0));
}

void RenderWindow::OnWindowResize(Vector2i newWindowSize)
{
    UiWindow::OnWindowResize(newWindowSize);
}


void RenderWindow::Initialize(Vector2i size)
{
    using namespace XnorCore;
    const std::vector<RenderTargetInfo> attachementsType =
    {
        {Attachment::Color01,true},
    };
    
    // Init Rendering
    m_FrameBuffer = FrameBuffer(size);
    m_ColorTexture = new Texture(TextureInternalFormat::Rgb16, m_FrameBuffer.GetSize());

    // Set Up renderPass
    const RenderPass renderPass(attachementsType);
    const std::vector<const Texture*> targets = { m_ColorTexture};
    m_FrameBuffer.Create(renderPass,targets);

    // Init rendererContext
    m_RendererContext.camera = &m_Editor->data.gameCam;
    m_RendererContext.framebuffer = &m_FrameBuffer;
}


