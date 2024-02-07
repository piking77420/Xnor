#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "utils/logger.hpp"

int main(int argc, char** argv)
{
	Logger::OpenDefaultFile();

	/*glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "XNOR Engine", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1); // Enable vsync
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable viewports

	io.Fonts->AddFontDefault();

	const char* const glslVersion = "#version 460";

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glslVersion);
	
	
	while (!glfwWindowShouldClose(window))
	{	
		glfwPollEvents();
		
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("hello");
		ImGui::End();

		ImGui::Render();

		int displayW, displayH;
		glfwGetFramebufferSize(window, &displayW, &displayH);
		glViewport(0, 0, displayW, displayH);
		
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		GLFWwindow* ctxBackup = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(ctxBackup);

		glfwSwapBuffers(window);
	}

	ImGui::DestroyPlatformWindows();

	ImGui::DestroyContext();

	glfwDestroyWindow(window);

	glfwTerminate();*/

	Logger::Stop();
	
	return 0;
}
