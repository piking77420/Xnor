#include "editor.hpp"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_impl_glfw.h>

void Editor::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Editor::EndFrame(const Window& window)
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	window.SetCurrentContext();
}

Editor::Editor(const Window& window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable viewports

	io.Fonts->AddFontDefault();

	const char* const glslVersion = "#version 460";

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(const_cast<GLFWwindow*>(window.GetWindow()), true);
	ImGui_ImplOpenGL3_Init(glslVersion);
}

Editor::~Editor()
{
	ImGui::DestroyPlatformWindows();
	ImGui::DestroyContext();
}

