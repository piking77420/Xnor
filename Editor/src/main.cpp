#include "imgui/imgui.h"
#include "rendering/opengl/opengl_renderer.hpp"
#include "scene/scene.hpp"
#include "utils/logger.hpp"
#include "editor.hpp"
#include "window.hpp"
#include "scene/component.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	using namespace XnorCore;
	using namespace XnorEditor;
	
	Logger::OpenDefaultFile();

	Window window;
	Editor editor(window);
	const std::unique_ptr<Renderer> renderer = std::make_unique<OpenGlRenderer>();

	Vector4 colorCheck = 0.5f;

	// Template seems to work between dll and exe
	Scene::CreateBasicScene();
	//Scene& scene = *Scene::scene;

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
