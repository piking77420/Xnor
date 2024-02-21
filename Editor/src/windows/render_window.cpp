#include "windows/render_window.hpp"

using namespace XnorCore;
using namespace XnorEditor;

RenderWindow::RenderWindow(Editor* editor,XnorCore::RendererContext* rendererContext) : UiWindow(editor)
{
    if(rendererContext == nullptr)
    {
        Logger::LogError("renderPass Context is Null");
        throw std::runtime_error("renderPass Context is Null");
    }

    if(rendererContext->IsEditor)
    {
        m_DisplayName = "Editor Window";
    }
    else
    {
        m_DisplayName = "GameWindow";
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
    ImGui::Begin(m_DisplayName.c_str());
    ImGui::Image(XnorCore::Utils::IntToPointer<ImTextureID>(m_ColorTexture->GetId()), ImGui::GetContentRegionAvail());
    ImGui::End();
}
