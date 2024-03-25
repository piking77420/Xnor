#include "editor.hpp"
#include "reflection//factory.hpp"
#include "utils/coroutine.hpp"
#include "utils/logger.hpp"

// https://www.reddit.com/r/opengl/comments/unc3fy/how_to_programatically_set_the_gpu_to_my_opengl/?onetap_auto=true&one_tap=true 
extern "C"
{
	_declspec(dllexport) unsigned long NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

/*using CoroutineT = XnorCore::Coroutine<>;

CoroutineT WaitRoutine()
{
	using namespace std::chrono_literals;
	
	co_await 700ms;

	co_return;
}*/

int main(int, char**)
{
	using namespace XnorCore;
	using namespace XnorEditor;

	Logger::OpenDefaultFile();

	//CoroutineT coroutine = WaitRoutine();

	Factory::RegisterTypes();

	Editor editor;

	editor.Update();

	return EXIT_SUCCESS;
}
