#include "windows/footer_window.hpp"

#include "csharp/dotnet_constants.hpp"
#include "csharp/dotnet_runtime.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorEditor;

FooterWindow::FooterWindow(Editor* editor)
    : UiWindow(editor, "Footer")
    , m_ScriptsWatcher(XnorCore::Dotnet::GameProjectLocation)
{
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus;

    m_BuildTexture = XnorCore::ResourceManager::Get<XnorCore::Texture>("assets_internal/editor/ui/footer/build_button.png");

    m_LastBuildSuccessTexture = XnorCore::ResourceManager::Get<XnorCore::Texture>("assets_internal/editor/ui/footer/build_success.png");
    m_LastBuildWarningTexture = XnorCore::ResourceManager::Get<XnorCore::Texture>("assets_internal/editor/ui/footer/build_warning.png");
    m_LastBuildErrorTexture = XnorCore::ResourceManager::Get<XnorCore::Texture>("assets_internal/editor/ui/footer/build_error.png");

    XnorCore::Pointer<XnorCore::File> imguiFont = XnorCore::FileManager::Get<XnorCore::File>("assets_internal/editor/ui/fonts/arial.ttf");
    const ImGuiIO& io = ImGui::GetIO();
    ImFontConfig fontConfig;
    fontConfig.FontDataOwnedByAtlas = false; // Make sure ImGui doesn't delete the data because we already do it
    m_ProjectTextFont = io.Fonts->AddFontFromMemoryTTF(imguiFont->GetData<void>(), static_cast<int32_t>(imguiFont->GetSize()), 25.f, &fontConfig);

    m_ScriptsWatcher.fileExtensions.AddRange({ ".cs", ".cs~" }); // For some reason sometimes the modified C# files end with a tilde '~'

    // Get updated when the game project scripts get modified in any way
    m_ScriptsWatcher.onCreated += [this](auto) { scriptsUpToDate = false; };
    m_ScriptsWatcher.onDeleted += [this](auto) { scriptsUpToDate = false; };
    m_ScriptsWatcher.onModified += [this](auto) { scriptsUpToDate = false; };
    m_ScriptsWatcher.onRenamed += [this](auto) { scriptsUpToDate = false; };

    m_Editor->onScriptsReloadingBegin += [this] { scriptsUpToDate = true; };

    m_ScriptsWatcher.Start();
}

FooterWindow::~FooterWindow()
{
    m_ScriptsWatcher.Stop();
}

void FooterWindow::Display()
{
    XnorCore::Pointer<XnorCore::Texture> lastBuildResultTexture;
    
    switch (XnorCore::DotnetRuntime::GetProjectLastBuildResult())
    {
        case XnorCore::DotnetRuntime::BuildResult::Unknown:
            break;
        
        case XnorCore::DotnetRuntime::BuildResult::Success:
            lastBuildResultTexture = m_LastBuildSuccessTexture;
            break;
        
        case XnorCore::DotnetRuntime::BuildResult::Warning:
            lastBuildResultTexture = m_LastBuildWarningTexture;
            break;
        
        case XnorCore::DotnetRuntime::BuildResult::Error:
            lastBuildResultTexture = m_LastBuildErrorTexture;
            break;
    }

    if (lastBuildResultTexture)
    {
        ImGui::Image(XnorCore::Utils::IntToPointer<ImTextureID>(lastBuildResultTexture->GetId()), { 25.f, 25.f });

        ImGui::SameLine();
    }

    ImGui::PushFont(m_ProjectTextFont);
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
        ImGui::Dummy({ ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(reloadingProjectText).x + 200.f + 60.f), 1.f });
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
        
        ImGui::ProgressBar(progress, { 200.f, 0.f });
        
        if (!reloadingScripts)
            ImGui::PopStyleVar();
    }
    else if (!scriptsUpToDate)
    {
        constexpr const char_t* const pleaseRecompileText = "Scripts have been modified, consider recompiling";
        ImGui::SameLine();
        ImGui::Dummy({ ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(pleaseRecompileText).x + 60.f), 1.f });
        ImGui::SameLine();
        ImGui::TextColored(static_cast<Vector4>(XnorCore::Color::OrangeRed()), pleaseRecompileText);
    }
    ImGui::PopFont();
    
    ImGui::SameLine();
    ImGui::Dummy({ ImGui::GetContentRegionAvail().x - 40.f, 1.f });

    ImGui::SameLine();
    
    if (reloadingScripts)
        ImGui::BeginDisabled();

    const bool_t buildButton = ImGui::ImageButton(
            XnorCore::Utils::IntToPointer<ImTextureID>(m_BuildTexture->GetId()),
            {25.f, 25.f},
            {},
            {1.f, 1.f},
            -1,
            static_cast<Vector4>(XnorCore::Color::Black()),
            scriptsUpToDate ? static_cast<Vector4>(XnorCore::Color::White()) : static_cast<Vector4>(XnorCore::Color::OrangeRed())
        );

    ImGui::SetItemTooltip("Build and reload C# scripts");
    
    if ((m_Editor->IsReloadScriptsOnSave() && !scriptsUpToDate) || buildButton)
        m_Editor->BuildAndReloadCodeAsync();
    
    if (reloadingScripts)
        ImGui::EndDisabled();
}
