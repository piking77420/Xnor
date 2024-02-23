#include "editor.hpp"
#include "file/file_manager.hpp"
#include "rendering/light/light.hpp"
#include "resource/resource_manager.hpp"
#include "scene/component/mesh_renderer.hpp"
#include "utils/list.hpp"
#include "utils/logger.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	using namespace XnorCore;
	using namespace XnorEditor;

#ifdef _DEBUG
	// Enable Visual Studio memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(POS);
#endif

	Logger::OpenDefaultFile();

	FileManager::LoadDirectory("assets");

	Editor editor;

	editor.Update();

	ResourceManager::RemoveAll();

	FileManager::UnloadAll();
	Logger::Stop();

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	
	return 0;
}
