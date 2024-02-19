#include "input/core_input.hpp"

using namespace XnorCore;

bool CoreInput::GetKey(KeyCode keyCode, KeyStatus keyStatus)
{
    return true;
}

CoreInput::CoreInput()
{
    m_InputQueue = new std::vector<InputKey>();
}

CoreInput::~CoreInput()
{
    delete m_InputQueue;
}

void CoreInput::HandleInputsEvents(const Window& window)
{
}
