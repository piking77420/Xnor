#include "windows/header_window.hpp"

#include <ImGui/imgui.h>

#include "file/file_manager.hpp"
#include "resource/resource_manager.hpp"
#include "scene/scene.hpp"
#include "utils/utils.hpp"

using namespace XnorEditor;
using namespace XnorCore;

HeaderWindow::HeaderWindow()
{
    PauseButtonPng = FileManager::Get("assets/editor/PauseButton.png");
    PauseButton =  ResourceManager::Load<Texture>(PauseButtonPng);
    
    PlayButtonPng = FileManager::Get("assets/editor/PlayButton.png");
    PlayButton = ResourceManager::Load<Texture>(PlayButtonPng);

    StopButtonPng = FileManager::Get("assets/editor/StopButton.png");
    StopButton = ResourceManager::Load<Texture>(StopButtonPng);
}

void HeaderWindow::Display()
{
    ImGui::Begin("HeaderWindow",nullptr,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
    
    // Value to offset the little gray image fro imgui
    float constantOffsetValue = 1.5f;
    int nbrOffImage = 2;

    m_ImageSize = m_TileBaseSize;
    
    for (int i = 0; i < nbrOffImage; i++)
    {
        float offSet = m_ImageSize * (nbrOffImage - (i + 1)) * constantOffsetValue;
        float x = ((ImGui::GetWindowSize().x - m_ImageSize) * 0.5f);
        float y = ((ImGui::GetWindowSize().y - m_ImageSize) * 0.5f);

        m_ImagePos[i].x = x - offSet;
        m_ImagePos[i].y = y;
    }
    
    DisplayOnEditor();
    DisplayOnPlay();
 
    ImGui::End();
}

void HeaderWindow::DisplayOnEditor()
{
    ImVec2 CurrentimagePos;
    
    CurrentimagePos = {m_ImagePos[0].x,m_ImagePos[0].y};
    ImGui::SetCursorPos(CurrentimagePos);
    if (ImGui::ImageButton((ImTextureID)PlayButton->GetId(),{m_ImageSize,m_ImageSize},{0,0},{1.f,1.f}))
    {
        Scene::IsBegin = true;
    }
    
}

void HeaderWindow::DisplayOnPlay()
{
    ImVec2 CurrentimagePos;
    
    if(!Scene::IsBegin)
        return;
    
    CurrentimagePos = {m_ImagePos[1].x,m_ImagePos[1].y};
    ImGui::SetCursorPos(CurrentimagePos);
    if(ImGui::ImageButton((ImTextureID)PauseButton->GetId(),{m_ImageSize,m_ImageSize},{0,0},{1.f,1.f}))
    {
        Scene::IsBegin = false;
        return;
    }
    
    CurrentimagePos = {m_ImagePos[0].x,m_ImagePos[0].y};
    ImGui::SetCursorPos(CurrentimagePos);
    if(ImGui::ImageButton((ImTextureID)StopButton->GetId(),{m_ImageSize,m_ImageSize},{0,0},{1.f,1.f}))
    {
        // Reload Scene
    }
    
}
