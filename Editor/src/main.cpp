#include "editor.hpp"
#include "file/file_manager.hpp"
#include "rendering/light/light.hpp"
#include "resource/resource_manager.hpp"
#include "scene/component/mesh_renderer.hpp"
#include "utils/list.hpp"
#include "utils/logger.hpp"
#include <iostream>

int main(int, char**)
{
	using namespace XnorCore;
	using namespace XnorEditor;

	Logger::OpenDefaultFile();

	FileManager::LoadDirectory("assets");

	Editor editor;

	editor.Update();

	ResourceManager::RemoveAll();

	FileManager::UnloadAll();
	
	Logger::Stop();

	return 0;
}
