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
	Vector4 clearColor = { 0.5f,0.5f,0.5f,0.5f};
	renderer.SetClearColor(clearColor);

	while (!window.ShouldClose())
	{
		window.PollEvents();
		editor.BeginFrame();
		renderer.ClearColorAndDepth();

		if (ImGui::Begin("Inspector"))
		{
			ImGui::End();
		}

		editor.EndFrame(window);
		window.SwapBuffers(); 
	}

	Logger::Stop();
	
	return 0;
}
