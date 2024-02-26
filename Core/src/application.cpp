#include "application.hpp"

#include "screen.hpp"
#include "input/input.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorCore;

Application::Application()
{
    m_ApplicationInstance = this;
    
    Window::Initialize();

    RHI::Initialize();
    ResourceManager::LoadAll();
    renderer.Initialize();
	
    Input::Initialize();
    Screen::Initialize();
}

Application::~Application()
{
    Window::Shutdown();

    ResourceManager::UnloadAll();

    FileManager::UnloadAll();
	
    Logger::Stop();
}

