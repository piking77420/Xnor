#include "file/file_manager.hpp"
#include "utils/logger.hpp"
#include "editor.hpp"

#include "physics/physics_world.hpp"

// https://www.reddit.com/r/opengl/comments/unc3fy/how_to_programatically_set_the_gpu_to_my_opengl/?onetap_auto=true&one_tap=true 
extern "C"
{
	_declspec(dllexport) unsigned long NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int32_t main(int32_t, char_t**)
{
	using namespace XnorCore;
	using namespace XnorEditor;

	Logger::OpenDefaultFile();

	PhysicsWorld::Initialize();
	// PhysicsWorld::CreateSphere(Vector3(5.f, 4.f, 3.f), 1.f);

	FileManager::LoadDirectory("assets_internal");
	FileManager::LoadDirectory("assets");

	Editor editor;

	editor.Update();

	PhysicsWorld::Destroy();

	return EXIT_SUCCESS;
}
