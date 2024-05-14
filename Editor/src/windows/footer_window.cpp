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

    const bool_t reloadingScripts = m_Editor->IsReloadingScripts();

    static std::chrono::time_point<std::chrono::system_clock> lastReloadingScriptsTrue;
    const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();

    if (reloadingScripts)
        lastReloadingScriptsTrue = now;

    const std::chrono::duration<float_t> diff = now - lastReloadingScriptsTrue;

    if (reloadingScripts || diff < FadeOutDuration)
    {
        if (!reloadingScripts)
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.f - diff / FadeOutDuration);
        
        constexpr const char_t* const reloadingProjectText = "Reloading .NET project";
        ImGui::SameLine();
        ImGui::Dummy({ ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(reloadingProjectText).x + 100.f + 60.f), 1.f });
        ImGui::SameLine();
        ImGui::Text(reloadingProjectText);
        ImGui::SameLine();
        
        float_t progress = 0.f;
        if (!m_Editor->IsSerializing())
        {
            const float_t reloadingProgress = XnorCore::DotnetRuntime::GetProjectReloadingProgress();
            progress = 0.1f + reloadingProgress * 0.8f;
            
            if (reloadingProgress >= 1.f && !m_Editor->IsDeserializing())
                progress = 1.f;
        }
        
        ImGui::ProgressBar(progress, { 100.f, 0.f });
        
        if (!reloadingScripts)
            ImGui::PopStyleVar();
    }
    
    ImGui::SameLine();
    ImGui::Dummy({ ImGui::GetContentRegionAvail().x - 40.f, 1.f });

    ImGui::SameLine();
    
    if (reloadingScripts)
        ImGui::BeginDisabled();
    
    if (ImGui::ImageButton(XnorCore::Utils::IntToPointer<ImTextureID>(m_BuildTexture->GetId()), { 25.f, 25.f }))
        m_Editor->BuildAndReloadCodeAsync();
    
    if (reloadingScripts)
        ImGui::EndDisabled();
}
