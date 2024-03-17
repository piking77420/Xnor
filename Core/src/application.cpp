#include "application.hpp"

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
    
    Window::Initialize();

    Rhi::Initialize();
    ResourceManager::LoadAll();
    renderer.Initialize();
	
    Input::Initialize();
    Screen::Initialize();

	//DotnetRuntime::Initialize();
}

Application::~Application()
{
	//DotnetRuntime::Shutdown();
	
    ResourceManager::UnloadAll();
	Rhi::Shutdown();

    Window::Shutdown();
	
    FileManager::UnloadAll();
	
    Logger::Stop();
}

