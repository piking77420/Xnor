#include "imgui/imgui.h"
#include "rendering/renderer.hpp"
#include "scene/scene.hpp"
#include "utils/logger.hpp"
#include "editor.hpp"
#include "window.hpp"
#include "file/file_manager.hpp"
#include "resource/resource_manager.hpp"
#include "resource/texture.hpp"
#include "utils/utils.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	using namespace XnorCore;
	using namespace XnorEditor;

	Logger::OpenDefaultFile();

	FileManager::LoadDirectoryRecursive("assets");

	Entity entity;
	entity.Begin();

	Editor editor;
	Scene::CreateBasicScene();

	editor.GetInspector()->SetObject(&entity);
	editor.Update();

	ResourceManager::RemoveAll();
	delete Scene::scene;
	FileManager::UnloadAll();
	Logger::Stop();
	
	return 0;
}
