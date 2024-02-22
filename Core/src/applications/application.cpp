#include "applications/application.hpp"

#include "screen.hpp"

using namespace XnorCore;

Application::Application()
{
    m_ApplicationInstance = this;
    CoreInput::Initialize(window);
    Screen::Initialize();
}
