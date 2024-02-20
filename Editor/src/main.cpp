#include "editor.hpp"
#include "file/file_manager.hpp"
#include "resource/resource_manager.hpp"
#include "utils/logger.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	using namespace XnorCore;
	using namespace XnorEditor;

	Logger::OpenDefaultFile();

	FileManager::LoadDirectoryRecursive("assets");

	Editor editor;

	editor.Update();

	ResourceManager::RemoveAll();

	FileManager::UnloadAll();
	Logger::Stop();
	
	return 0;
}
