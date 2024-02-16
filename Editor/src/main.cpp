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

class TestClass : public XnorCore::Reflectable
{
public:
	REFLECTABLE_IMPL(TestClass)
	
private:
	float_t  m1[64];
	float_t  m2;
	float_t  m3;
	float_t* m4;
};

REFL_AUTO(type(TestClass),
	field(m1), field(m2),
	field(m3), field(m4)
)

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	using namespace XnorCore;
	using namespace XnorEditor;

	Logger::OpenDefaultFile();

	TestClass t;
	t.CreateTypeInfo();

	const TypeInfo& type = TypeInfo::Get<TestClass>();
	Logger::LogInfo("%s", type.GetName().c_str());

	const std::vector<FieldInfo>& members = type.GetMembers();

	for (const FieldInfo& f : members)
	{
		Logger::LogInfo("%s ; %s ; Hash : %llu ; Size : %llu ; %llu ; Is array : %d ; Is static : %d ; Is const : %d", f.typeName.c_str(), f.name.c_str(), f.typeHash, f.size, f.offset, f.isArray, f.isStatic, f.isConst);
		// Logger::LogInfo("Hash : %llu", f.typeHash);
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

		editor.EndFrame(window);
		window.SwapBuffers(); 
	}

	ResourceManager::DeleteAll();
	
	delete Scene::scene;

	FileManager::DeleteAll();

	Logger::Stop();
	
	return 0;
}
