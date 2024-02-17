#include "editor.hpp"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_impl_glfw.h>

using namespace XnorEditor;

Editor::Editor(XnorCore::Window& window)
	: m_Window(&window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	io.Fonts->AddFontDefault();

	const char* const glslVersion = "#version 460";

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(const_cast<GLFWwindow*>(static_cast<const GLFWwindow*>(window.GetWindow())), true);
	ImGui_ImplOpenGL3_Init(glslVersion);
}

void Editor::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Editor::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	m_Window->SetCurrentContext();
}

Editor::~Editor()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyPlatformWindows();
	ImGui::DestroyContext();
}
