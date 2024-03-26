#include "windows/header_window.hpp"

#include <ImGui/imgui.h>

#include "world/world.hpp"
#include "file/file_manager.hpp"
#include "resource/resource_manager.hpp"
#include "scene/scene.hpp"
#include "utils/utils.hpp"
#include "serialization/serializer.hpp"

using namespace XnorEditor;

HeaderWindow::HeaderWindow(Editor* editor)
    : UiWindow(editor, "Header")
{
    m_PauseButton = XnorCore::ResourceManager::Get<XnorCore::Texture>("assets_internal/editor/ui/pause_button.png");
    m_PlayButton = XnorCore::ResourceManager::Get<XnorCore::Texture>("assets_internal/editor/ui/play_button.png");
    m_StopButton = XnorCore::ResourceManager::Get<XnorCore::Texture>("assets_internal/editor/ui/stop_button.png");
    windowFlags = ImGuiWindowFlags_NoScrollbar;
}

void HeaderWindow::Display()
{
    // Value to offset the little gray image from imgui
    constexpr size_t nbrOfImages = 2;

    m_ImageSize = TileBaseSize;
    
    for (size_t i = 0; i < nbrOfImages; i++)
    {
        constexpr float_t constantOffsetValue = 1.5f;
        const float_t offSet = m_ImageSize * static_cast<float_t>(nbrOfImages - (i + 1)) * constantOffsetValue;
        const float_t x = (static_cast<float_t>(m_Size.x) - m_ImageSize) * 0.5f;
        const float_t y = (static_cast<float_t>(m_Size.y) - m_ImageSize) * 0.5f;

        m_ImagePos[i].x = x - offSet;
        m_ImagePos[i].y = y;
    }

    if (XnorCore::World::isPlaying)
        DisplayOnPlay();
    else
        DisplayOnEditor();
}

void HeaderWindow::DisplayOnEditor()
{
    const ImVec2 currentimagePos = { m_ImagePos[0].x,m_ImagePos[0].y };
    ImGui::SetCursorPos(currentimagePos);

    if (ImGui::ImageButton(XnorCore::Utils::IntToPointer<ImTextureID>(m_PlayButton->GetId()), { m_ImageSize, m_ImageSize }))
    {
        XnorCore::World::isPlaying = true;
        XnorCore::World::hasStarted = false;
    }
}

void HeaderWindow::DisplayOnPlay()
{
    ImVec2 currentimagePos = { m_ImagePos[1].x, m_ImagePos[1].y };

    ImGui::SetCursorPos(currentimagePos);

    if (ImGui::ImageButton(XnorCore::Utils::IntToPointer<ImTextureID>(m_PauseButton->GetId()), { m_ImageSize, m_ImageSize }))
    {
        XnorCore::World::isPlaying = false;
        return;
    }
    
    currentimagePos = { m_ImagePos[0].x, m_ImagePos[0].y };
    ImGui::SetCursorPos(currentimagePos);
    
    if (ImGui::ImageButton(XnorCore::Utils::IntToPointer<ImTextureID>(m_StopButton->GetId()), { m_ImageSize, m_ImageSize }))
    {
        std::string path;
        if (m_Editor->data.currentScene == nullptr)
        {
            if (!std::filesystem::exists("assets/scenes"))
                std::filesystem::create_directories("assets/scenes");
            path = Editor::SerializedScenePath;
        }
        else
        {
            path = m_Editor->data.currentScene->GetPathString();
        }

        XnorCore::Serializer::StartDeserialization(path);
        m_Editor->data.selectedEntity = nullptr;
        delete XnorCore::World::scene;
        XnorCore::World::scene = new XnorCore::Scene();
        // Possible memory leak?
        XnorCore::Serializer::Deserialize<XnorCore::Scene, true>(XnorCore::World::scene);
        // XnorCore::Serializer::Deserialize<XnorCore::TestComponent, true>(v[0]);
        XnorCore::Serializer::EndDeserialization();

        XnorCore::World::isPlaying = false;
        XnorCore::World::hasStarted = false;

        m_Editor->renderer.BeginFrame(*XnorCore::World::scene);
    }
}
