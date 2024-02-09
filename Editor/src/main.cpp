#include "imgui/imgui.h"
//#include "rendering/opengl/opengl_renderer.hpp"
#include "utils/logger.hpp"
#include "Editor.hpp"
#include "window.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	Logger::OpenDefaultFile();

	Window window;
	Editor editor = Editor(window);
	//OpenglRenderer renderer;

	while (!window.ShouldClose())
	{
		window.PollEvents();
		editor.BeginFrame();

		if (ImGui::Begin("Window"))
		{
			ImGui::End();
		}

		editor.EndFrame(window);
		window.SwapBuffers(); 
	}

	Logger::Stop();
	
	return 0;
}
