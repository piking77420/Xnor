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

	EntityTest t;
	t.Begin();
	EntityDamage d;
	d.Begin();

	const TypeInfo& type = TypeInfo::Get<Entity>();

	const std::vector<FieldInfo>& members = type.GetMembers();

	for (const FieldInfo& f : members)
	{
		Logger::LogInfo("%s ; %s ; Hash : %llu ; Size : %llu ; Element size : %llu ; Offset : %llu ; Is array : %d ; Is static : %d ; Is const : %d", f.typeName.c_str(), f.name.c_str(), f.typeHash, f.fullSize, f.elementSize, f.offset, f.isArray, f.isStatic, f.isConst);
	}
	
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
