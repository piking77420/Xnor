// ReSharper disable CppClangTidyCppcoreguidelinesPreferMemberInitializer
#include "windows/animation_montage_window.hpp"
#include "resource/animation_montage.hpp"

using namespace XnorEditor;

AnimationMontageWindow::AnimationMontageWindow(Editor* editor)
    : UiWindow(editor, "AnimationMontageEditor")
{
    m_Scene.CreateEntity("Montage");
    m_Entity = m_Scene.GetEntities()[0];

    m_Entity->AddComponent<XnorCore::SkinnedMeshRenderer>();

    m_Viewport.Init(XnorCore::Window::GetSize());
}

void AnimationMontageWindow::Display()
{
    if (m_Montage == nullptr)
    {
        ImGui::Text("No montage selected");
        return;
    }

    ImGui::Image(XnorCore::Utils::IntToPointer<ImTextureID>(m_Viewport.image->GetId()), ImGui::GetContentRegionAvail(),  ImVec2(0, 1), ImVec2(1, 0));
}

void AnimationMontageWindow::OnApplicationRendering()
{
    const bool_t isValid = m_Viewport.IsValid();
    
    if (!isValid || m_Viewport.viewPortSize != m_Size)
    {
        m_Viewport.Resize(m_Size);
    }

    if (isValid)
        m_Editor->renderer.RenderViewport(m_Viewport, m_Scene);
}

void AnimationMontageWindow::SetParam(void* param)
{
    m_Montage = static_cast<decltype(m_Montage)>(param);

    m_Montage->Start();
}
