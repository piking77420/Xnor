#include "windows/header_window.hpp"

#include <ImGui/imgui.h>

#include "utils/utils.hpp"

using namespace XnorEditor;

void HeaderWindow::Display()
{
    ImGui::Begin("HeaderWindow ");

    XnorCore::Utils::CenterImguiObject();
    
    ImGui::ArrowButton("Button Play", ImGuiDir_Right);
    
    ImGui::End();
}
