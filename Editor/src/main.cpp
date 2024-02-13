#include "imgui/imgui.h"
#include "rendering/renderer.hpp"
#include "scene/scene.hpp"
#include "utils/logger.hpp"
#include "editor.hpp"
#include "window.hpp"
#include "file/file_manager.hpp"
#include "resource/resource_manager.hpp"
#include "resource/texture.hpp"
#include "scene/component.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	using namespace XnorCore;
	using namespace XnorEditor;
	
	Logger::OpenDefaultFile();

	Pointer<File> gradientFile = FileManager::Load("assets/gradient.png");

	//Pointer<Texture> gradient = ResourceManager::Create<Texture>("gradient");
	//gradient->Load(*gradientFile);

	Window window;
	Editor editor(window);
	Renderer renderer;

	Vector4 colorCheck = 0.5f;
	// Template seems to work between dll and exe
	Scene::CreateBasicScene();
	//Scene& scene = *Scene::scene;
	RendererContext context;

	while (!window.ShouldClose())
	{
		window.PollEvents();
		editor.BeginFrame();
		
		renderer.SetClearColor(colorCheck);
		renderer.ClearColorAndDepth();

		ImGui::ColorPicker4("colorPickerTest", colorCheck.Raw(), ImGuiColorEditFlags_PickerHueWheel);

		renderer.RenderScene(*Scene::scene, context);

		editor.EndFrame(window);
		window.SwapBuffers(); 
	}

	//ResourceManager::Delete("gradient");

	FileManager::Delete(gradientFile);

	Logger::Stop();
	
	return 0;
}
