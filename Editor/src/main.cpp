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

	Window window;
	Editor editor(window);
	Renderer renderer;

	
	Scene::CreateBasicScene();
	//Scene& scene = *Scene::scene;
	RendererContext context;
	Camera cam;
	context.camera = &cam;

	
	while (!window.ShouldClose())
	{
		window.PollEvents();
		editor.BeginFrame();

		ImGui::Begin("Renderer Settings");
		if(ImGui::Button("Recompile Shader"))
		{
			renderer.CompileShader();
		}
		ImGui::End();
		
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
