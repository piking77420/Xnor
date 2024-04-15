#include "application.hpp"

#include "screen.hpp"
#include "csharp/dotnet_runtime.hpp"
#include "file/file_manager.hpp"
#include "input/input.hpp"
#include "physics/physics_world.hpp"
#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"

#include "audio/audio.hpp"
#include "utils/message_box.hpp"

using namespace XnorCore;

void Application::Exit(const int32_t code)
{
	Logger::LogInfo("Force exiting Application");

	delete applicationInstance;
	
	std::exit(code);  // NOLINT(concurrency-mt-unsafe)
}

Application::Application(const int32_t, const char_t* const* const argv)
{
    applicationInstance = this;
	

	executablePath = argv[0];

	Logger::Start();
    
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
	if (!Audio::Initialize())
		Logger::LogError("Couldn't initialize audio");

	if (!DotnetRuntime::Initialize())
	{
		const auto result = MessageBox::Show("Error", "Couldn't initialize .NET runtime. Continue execution anyway ?", MessageBox::Type::YesNo, MessageBox::Icon::Error, MessageBox::DefaultButton::Second);
		if (result == MessageBox::Result::No)
			Exit(EXIT_FAILURE);
	}

	World::scene = new Scene;

	if (!DotnetRuntime::LoadAssembly("Game"))
		Logger::LogWarning("Couldn't load assembly Game.dll");

	gameViewPort = new Viewport();
}

Application::~Application()
{
	delete gameViewPort;
	delete World::scene;
	
	DotnetRuntime::Shutdown();

	Audio::Shutdown();
	PhysicsWorld::Destroy();
	
    ResourceManager::UnloadAll();
	Rhi::Shutdown();

    Window::Shutdown();
	
    FileManager::UnloadAll();

	Logger::Stop();
}
