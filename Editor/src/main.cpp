#include "file/file_manager.hpp"
#include "utils/logger.hpp"

#include "editor.hpp"

int main(int, char**)
{
	XnorCore::Logger::OpenDefaultFile();

	XnorCore::FileManager::LoadDirectory("assets");
	
	XnorEditor::Editor editor;

	editor.Update();

	return 0;
}
