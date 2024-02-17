#include "imgui/imgui.h"
#include "rendering/renderer.hpp"
#include "scene/scene.hpp"
#include "utils/logger.hpp"
#include "editor.hpp"
#include "window.hpp"
#include "file/file_manager.hpp"
#include "resource/model.hpp"
#include "resource/resource_manager.hpp"
#include "resource/texture.hpp"
#include "utils/memory_leak_detector.hpp"
#include "utils/reflectable.hpp"
#include "entity_test.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	using namespace XnorCore;
	using namespace XnorEditor;

	Logger::OpenDefaultFile();
	
	//MemoryLeakDetector detector;

	const Pointer<File> modelFile = FileManager::Load("assets/models/sphere.obj");
	
	Window window;
	Editor editor(window);
	Renderer renderer;

	Pointer<Model> model = ResourceManager::CreateAndLoad<Model>(modelFile);
	
	Scene::CreateBasicScene();
	//Scene& scene = *Scene::scene;
	RendererContext context;

	while (!window.ShouldClose())
	{
		window.PollEvents();
		editor.BeginFrame();

		ImGui::ColorPicker4("colorPickerTest", renderer.clearColor.Raw(), ImGuiColorEditFlags_PickerHueWheel);
		editor.Update();

		renderer.RenderScene(*Scene::scene, context);

		editor.EndFrame();
		window.SwapBuffers(); 
	}

	ResourceManager::DeleteAll();
	
	delete Scene::scene;

	FileManager::DeleteAll();

	Logger::Stop();
	
	return 0;
}
