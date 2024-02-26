#include "file/file_manager.hpp"
#include "utils/logger.hpp"
#include "editor.hpp"

int main(int, char**)
{
	using namespace XnorCore;
	using namespace XnorEditor;
	
	Logger::OpenDefaultFile();

	FileManager::LoadDirectory("assets_internal");
	FileManager::LoadDirectory("assets");

	Editor editor;

	editor.Update();

	return 0;
}
