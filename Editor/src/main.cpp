#include "imgui/imgui.h"
#include "rendering/opengl/opengl_renderer.hpp"
#include "utils/logger.hpp"
#include "Editor.hpp"
#include "window.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	Logger::OpenDefaultFile();

	Window window;
	Editor editor = Editor(window);
	OpenglRenderer renderer;

	Vector4 colorCheck = { 0.5f,0.5f,0.5f,0.5f};

	while (!window.ShouldClose())
	{
		window.PollEvents();
		editor.BeginFrame();

		renderer.SetClearColor(colorCheck);
		renderer.ClearColorAndDepth();

		ImGui::ColorPicker4("colorPickerTest", colorCheck.Raw(), ImGuiColorEditFlags_PickerHueWheel);


		editor.EndFrame(window);
		window.SwapBuffers(); 
	}

	Logger::Stop();
	
	return 0;
}
