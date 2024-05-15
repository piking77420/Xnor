#include "editor.hpp"
#include "reflection/dotnet_reflection.hpp"
#include "reflection/xnor_factory.hpp"
#include "utils/file_system_watcher.hpp"
#include "utils/logger.hpp"
#include "utils/windows.hpp"

// https://www.reddit.com/r/opengl/comments/unc3fy/how_to_programatically_set_the_gpu_to_my_opengl/?onetap_auto=true&one_tap=true 
extern "C"
{
	_declspec(dllexport) uint32_t NvOptimusEnablement = 1;
	_declspec(dllexport) int32_t AmdPowerXpressRequestHighPerformance = 1;
}


int32_t main(const int32_t argc, const char_t* const* const argv)
{
	using namespace XnorCore;
	using namespace XnorEditor;

	Logger::OpenDefaultFile();
	XnorFactory::RegisterAllTypes();
	DotnetReflection::RegisterAllTypes();

	Editor editor(std::forward<decltype(argc)>(argc), std::forward<decltype(argv)>(argv));

	editor.Update();

	return EXIT_SUCCESS;
}
