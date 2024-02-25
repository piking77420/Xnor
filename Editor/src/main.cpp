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
	XnorCore::Logger::OpenDefaultFile();

	XnorCore::FileManager::LoadDirectory("assets");
	
	XnorEditor::Editor editor;

	editor.Update();

	return 0;
}
