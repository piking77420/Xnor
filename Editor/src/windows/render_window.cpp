#include "windows/render_window.hpp"

using namespace XnorEditor;

RenderWindow::RenderWindow(Editor* editor)
    : UiWindow(editor, "Game Preview")
{
    Initialize(&m_RendererContext);
}

RenderWindow::RenderWindow(Editor* editor, const std::string& title)
    : UiWindow(editor, title)
{
    Initialize(&m_RendererContext);
}

RenderWindow::~RenderWindow()
{
    delete m_ColorTexture;
}

void RenderWindow::Display()
{
    if (XnorCore::World::world != nullptr)
    {
        m_Editor->renderer.RenderScene(XnorCore::World::world->Scene, m_RendererContext);
    }
    
    ImGui::Image(XnorCore::Utils::IntToPointer<ImTextureID>(m_ColorTexture->GetId())
        , ImGui::GetContentRegionAvail(),
        {1,1},{0,0}
        );
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
    };
    
    // Init Rendering
    m_FrameBuffer = FrameBuffer(Window::GetSize());
    m_ColorTexture = new Texture(AttachementsType::Color, m_FrameBuffer.GetSize());
    
    // Set Up renderPass
    const RenderPass renderPass(attachementsType);
    const std::vector targets = { m_ColorTexture };
    m_FrameBuffer.Create(renderPass,targets);

    // Init rendererContext
    rendererContext->camera = &m_Editor->data.gameCam;
    rendererContext->framebuffer = &m_FrameBuffer;
    rendererContext->renderPass = &m_RenderPass;
}
