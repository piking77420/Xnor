#include "editor.hpp"
#include "reflection/xnor_factory.hpp"
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

	Editor editor(std::forward<const int32_t>(argc), std::forward<const char_t* const* const>(argv));

	editor.Update();

	Logger::LogInfo("Repeating log");
	Logger::LogInfo("Repeating log");
	Logger::LogInfo("Repeating log");
	Logger::LogInfo("Repeating log");
	Logger::LogInfo("Repeating log");
	Logger::LogInfo("Repeating log");

	return EXIT_SUCCESS;
}
