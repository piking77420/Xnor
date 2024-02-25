#include "applications/application.hpp"
#include "input/core_input.hpp"
#include "screen.hpp"

using namespace XnorCore;

Application::Application()
{
    m_ApplicationInstance = this;
    Window::Initialize();
    renderer.Initialize();
    CoreInput::Initialize();
    Screen::Initialize();
}

Application::~Application()
{
    Window::Shutdown();
}
