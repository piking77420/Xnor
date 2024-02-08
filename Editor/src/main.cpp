#include "imgui/imgui.h"

#include "utils/logger.hpp"
#include "Editor.hpp"
#include "window.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	Logger::OpenDefaultFile();

	Window window = Window();
	Editor editor = Editor(window);

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
