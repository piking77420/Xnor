#include "imgui/imgui.h"
#include "rendering/renderer.hpp"
#include "scene/scene.hpp"
#include "utils/logger.hpp"
#include "editor.hpp"
#include "window.hpp"
#include "file/file_manager.hpp"
#include "resource/resource_manager.hpp"
#include "resource/texture.hpp"
#include "utils/memory_leak_detector.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	using namespace XnorCore;
	using namespace XnorEditor;
	
	Logger::OpenDefaultFile();
	
	//MemoryLeakDetector detector;

	Pointer<File> gradientFile = FileManager::Load("assets/textures/gradient.png");

	Window window;
	Editor editor(window);
	Renderer renderer;

	Pointer<Texture> gradient = ResourceManager::CreateAndLoad<Texture>(FileManager::Get("assets/textures/gradient.png"));
	
	Vector4 colorCheck = 0.5f;
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

	ResourceManager::DeleteAll();

	FileManager::DeleteAll();

	Logger::Stop();
	
	return 0;
}
