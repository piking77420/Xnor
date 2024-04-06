#include "application.hpp"

#include "screen.hpp"
#include "csharp/dotnet_runtime.hpp"
#include "file/file_manager.hpp"
#include "input/input.hpp"
#include "physics/physics_world.hpp"
#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"

// We need windows.h for GetModuleFileNameA
#undef APIENTRY
// ReSharper disable once CppInconsistentNaming
#define XMLDocument XMLDocument_dont_care
#include <windows.h>
#undef XMLDocument

using namespace XnorCore;

Application::Application()
{
    m_ApplicationInstance = this;

	char_t exePath[MAX_PATH];
	GetModuleFileNameA(nullptr, exePath, MAX_PATH);
	executablePath = absolute(std::filesystem::path(exePath));
    
    Window::Initialize();

	Rhi::Initialize();

	Texture::defaultLoadOptions = { .flipVertically = true };
	FileManager::LoadDirectory("assets");
	FileManager::LoadDirectory("assets_internal/shaders");
	FileManager::LoadDirectory("assets_internal/editor/gizmos");
    ResourceManager::LoadAll();
	
	ResourceManager::LoadGuidMap();
	
    renderer.Initialize();
	
	PhysicsWorld::Initialize();
    Input::Initialize();
    Screen::Initialize();

	if (!DotnetRuntime::Initialize())
	{
		const int result = MessageBoxA(nullptr, "Couldn't initialize .NET runtime. Continue execution anyway ?", "Error", MB_YESNO | MB_ICONERROR | MB_DEFBUTTON2);
		if (result == IDNO)
			Exit(EXIT_FAILURE);
	}

	World::scene = new Scene;

	if (!DotnetRuntime::LoadAssembly("Game"))
		Logger::LogWarning("Couldn't load assembly Game.dll");
}

Application::~Application()
{
	delete World::scene;
	
	DotnetRuntime::Shutdown();

	PhysicsWorld::Destroy();
	
    ResourceManager::UnloadAll();
	Rhi::Shutdown();

    Window::Shutdown();
	
    FileManager::UnloadAll();
	
    Logger::Stop();
}

void Application::Exit(const int32_t code)
{
	Logger::LogInfo("Force exiting Application");
	std::exit(code);
}
