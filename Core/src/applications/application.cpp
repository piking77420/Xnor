#include "applications/application.hpp"

#include "screen.hpp"

using namespace XnorCore;

Application::Application()
{
    m_ApplicationInstance = this;
    Window::Initialize();
    CoreInput::Initialize();
    Screen::Initialize();
}

Application::~Application()
{
    Window::Shutdown();
}
