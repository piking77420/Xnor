#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "Maths/vector2.hpp"

#include "utils/logger.hpp"
#include "Editor.hpp"
#include "window.hpp"

int main(int argc, char** argv)
{
	Logger::OpenDefaultFile();

	Window window = Window();
	Editor editor = Editor(window);

	while (!window.ShouldClose())
	{
		window.PoolEvents();
		editor.BeginFrame();

		if (ImGui::Begin("sqdsqd"));
		{
			ImGui::End();
		}

		editor.EndFrame(window);
		window.SwapBuffers(); 
	}

	Logger::Stop();
	
	return 0;
}
