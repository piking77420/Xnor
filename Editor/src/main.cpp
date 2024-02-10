#include "imgui/imgui.h"
#include "rendering/opengl/opengl_renderer.hpp"
#include "scene/scene.hpp"
#include "utils/logger.hpp"
#include "Editor.hpp"
#include "window.hpp"


int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	Logger::OpenDefaultFile();

	Window window;
	Editor editor(window);
	std::unique_ptr<Renderer> renderer(std::make_unique<OpenglRenderer>());

	Vector4 colorCheck = { 0.5f,0.5f,0.5f,0.5f};
	
	
	Scene::CreateBasicScene();
	Scene& scene = *Scene::scene;

	while (!window.ShouldClose())
	{
		window.PollEvents();
		editor.BeginFrame();
		
		
		renderer->SetClearColor(colorCheck);
		renderer->ClearColorAndDepth();

		ImGui::ColorPicker4("colorPickerTest", colorCheck.Raw(), ImGuiColorEditFlags_PickerHueWheel);


		editor.EndFrame(window);
		window.SwapBuffers(); 
	}

	Logger::Stop();
	
	return 0;
}
