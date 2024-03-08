#include "file/file_manager.hpp"
#include "utils/logger.hpp"
#include "editor.hpp"

// https://www.reddit.com/r/opengl/comments/unc3fy/how_to_programatically_set_the_gpu_to_my_opengl/?onetap_auto=true&one_tap=true 
extern "C"
{
	_declspec(dllexport) unsigned long NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int main(int, char**)
{
	using namespace XnorCore;
	using namespace XnorEditor;
	
	Logger::OpenDefaultFile();

	using namespace std::string_literals;

	FileManager::LoadDirectory("assets_internal");
	FileManager::LoadDirectory("assets");

	Editor editor;

	editor.Update();

	return EXIT_SUCCESS;
}
