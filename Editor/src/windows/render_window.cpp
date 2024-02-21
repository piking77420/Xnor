#include "windows/render_window.hpp"

using namespace XnorCore;
using namespace XnorEditor;

RenderWindow::RenderWindow(Editor* editor,const std::string& name,XnorCore::RendererContext* rendererContext) : UiWindow(editor,name)
{
    if(rendererContext == nullptr)
    {
        Logger::LogError("renderPass Context is Null");
        throw std::runtime_error("renderPass Context is Null");
    }
    
    std::vector attachementsType =
    {
        AttachementsType::Color,
        AttachementsType::DepthAndStencil
    };
    
    // Init Rendering
    m_FrameBuffer = FrameBuffer(Window::GetSize());
    m_ColorTexture = new Texture(AttachementsType::Color, m_FrameBuffer.GetSize());
    m_DepthTexture = new Texture(AttachementsType::DepthAndStencil,m_FrameBuffer.GetSize());
    
    // Set Up renderPass
    RenderPass renderPass(attachementsType);
    std::vector targets = { m_ColorTexture, m_DepthTexture };
    m_FrameBuffer.Create(renderPass,targets);

    // Init rendererContext
    rendererContext->camera = &m_Camera;
    rendererContext->framebuffer = &m_FrameBuffer;
    rendererContext->renderPass = &m_RenderPass;
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
