#include "windows/footer_window.hpp"

#include "csharp/dotnet_constants.hpp"
#include "csharp/dotnet_runtime.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorEditor;

FooterWindow::FooterWindow(Editor* editor)
    : UiWindow(editor, "Footer")
{
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus;

    m_BuildTexture = XnorCore::ResourceManager::Get<XnorCore::Texture>("assets_internal/editor/ui/build_button.png");
}

void FooterWindow::Display()
{
    ImGui::Text("XNOR > %s", XnorCore::Dotnet::GameProjectName);

    const bool_t reloadingProject = XnorCore::DotnetRuntime::IsReloadingProject();

    if (reloadingProject)
    {
        constexpr const char_t* const reloadingProjectText = "Reloading .NET project";
        ImGui::SameLine();
        ImGui::Dummy({ ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(reloadingProjectText).x + 100.f + 60.f), 1.f });
        ImGui::SameLine();
        ImGui::Text(reloadingProjectText);
        ImGui::SameLine();
        ImGui::ProgressBar(XnorCore::DotnetRuntime::GetProjectReloadingProgress(), { 100.f, 0.f });
    }
    
    ImGui::SameLine();
    ImGui::Dummy({ ImGui::GetContentRegionAvail().x - 40.f, 1.f });

    ImGui::SameLine();
    
    if (reloadingProject)
        ImGui::BeginDisabled();
    
    if (ImGui::ImageButton(XnorCore::Utils::IntToPointer<ImTextureID>(m_BuildTexture->GetId()), { 25.f, 25.f }))
        m_Editor->BuildAndReloadCode();
    
    if (reloadingProject)
        ImGui::EndDisabled();
}
