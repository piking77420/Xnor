#include "windows/render_window.hpp"

using namespace XnorEditor;

RenderWindow::RenderWindow(Editor* editor, XnorCore::RendererContext* rendererContext)
    : UiWindow(editor, "Game Preview")
{
    Initialize(rendererContext);
}

RenderWindow::RenderWindow(Editor* editor, XnorCore::RendererContext* rendererContext, const std::string& title)
    : UiWindow(editor, title)
{
    Initialize(rendererContext);
}

RenderWindow::~RenderWindow()
{
    delete m_ColorTexture;
    delete m_DepthTexture;
}

void RenderWindow::Display()
{
    ImGui::Image(XnorCore::Utils::IntToPointer<ImTextureID>(m_ColorTexture->GetId()), ImGui::GetContentRegionAvail());
}

void RenderWindow::Initialize(XnorCore::RendererContext* rendererContext)
{
    using namespace XnorCore;
    
    if(rendererContext == nullptr)
    {
        Logger::LogError("renderPass Context is Null");
        throw std::runtime_error("renderPass Context is Null");
    }

    const std::vector attachementsType =
    {
        AttachementsType::Color,
        AttachementsType::DepthAndStencil
    };
    
    // Init Rendering
    m_FrameBuffer = FrameBuffer(Window::GetSize());
    m_ColorTexture = new Texture(AttachementsType::Color, m_FrameBuffer.GetSize());
    m_DepthTexture = new Texture(AttachementsType::DepthAndStencil,m_FrameBuffer.GetSize());
    
    // Set Up renderPass
    const RenderPass renderPass(attachementsType);
    const std::vector targets = { m_ColorTexture, m_DepthTexture };
    m_FrameBuffer.Create(renderPass,targets);

    // Init rendererContext
    rendererContext->camera = &m_Camera;
    rendererContext->framebuffer = &m_FrameBuffer;
    rendererContext->renderPass = &m_RenderPass;
}
