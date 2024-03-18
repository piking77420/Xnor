#include "application.hpp"

// We need windows.h for GetModuleFileNameA
#undef APIENTRY
// ReSharper disable once CppInconsistentNaming
#define XMLDocument XMLDocument_dont_care
#include <windows.h>
#undef XMLDocument

#include "screen.hpp"
#include "csharp/dotnet_runtime.hpp"
#include "file/file_manager.hpp"
#include "input/input.hpp"
#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorCore;

Application::Application()
{
    m_ApplicationInstance = this;

	char_t exePath[MAX_PATH];
	GetModuleFileNameA(nullptr, exePath, MAX_PATH);
	executablePath = absolute(std::filesystem::path(exePath));
    
    Window::Initialize();

    Rhi::Initialize();
    ResourceManager::LoadAll();
    renderer.Initialize();
	
    Input::Initialize();
    Screen::Initialize();

	DotnetRuntime::Initialize();

	World::scene = new Scene;
	DotnetRuntime::LoadAssembly("Game.dll");

}

Application::~Application()
{
	delete World::scene;
	
	DotnetRuntime::Shutdown();
	
    ResourceManager::UnloadAll();
	Rhi::Shutdown();

    Window::Shutdown();
	
    FileManager::UnloadAll();
	
    Logger::Stop();
}

