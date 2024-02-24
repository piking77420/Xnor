#include "input/core_input.hpp"

#include "assimp/Logger.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

bool CoreInput::GetKey(const Key key, const KeyStatus status)
{
    return m_Keyboard.at(static_cast<uint16_t>(key)).at(static_cast<uint16_t>(status));
}

bool CoreInput::GetMouseButton(const MouseButton mouseButton, const MouseButtonStatus status)
{
    return m_Mouse.at(static_cast<uint8_t>(mouseButton)).at(static_cast<uint8_t>(status));
}

bool CoreInput::GetGamePadButton(const uint32_t gamePadId, const GamepadButton gamepadButton, const GameButtonStatus buttonStatus)
{
    if(gamePadId > GamePadMax)
    {
        Logger::LogFatal("Gamepad Id none valid  = {}",gamePadId);
        throw std::exception("Gamepad Id none valid");
    }

    return m_GamePadsButton.at(gamePadId).at(static_cast<uint32_t>(gamepadButton)).at(static_cast<uint32_t>(buttonStatus));
}

void CoreInput::HandleEvent()
{
    HandleGamePad();
}

void CoreInput::HandleKeyboard(GLFWwindow*, const int32_t key, const int32_t, const int32_t action, const int32_t)
{
    KeyStatuses& keyStatuses = m_Keyboard.at(key);
    
    switch (action)
    {
        case GLFW_RELEASE:
            keyStatuses.at(static_cast<uint8_t>(KeyStatus::Down)) = false;
            keyStatuses.at(static_cast<uint8_t>(KeyStatus::Release)) = true;
            keyStatuses.at(static_cast<uint8_t>(KeyStatus::Repeat)) = false;
            break;
        
        case GLFW_PRESS:
            keyStatuses.at(static_cast<uint8_t>(KeyStatus::Press)) = true;
            keyStatuses.at(static_cast<uint8_t>(KeyStatus::Down)) = true;
            break;
        
        case GLFW_REPEAT:
            keyStatuses.at(static_cast<uint8_t>(KeyStatus::Repeat)) = true;
            break;
        
        default:
            break;
    }
}

void CoreInput::HandleMouseButton(GLFWwindow*, const int32_t mouseButton, const int32_t action, int32_t)
{
    MouseStatuses& keyStatuses = m_Mouse.at(mouseButton);
    
    switch (action)
    {
        case GLFW_RELEASE:
            keyStatuses.at(static_cast<uint8_t>(MouseButtonStatus::Down)) = false;
            keyStatuses.at(static_cast<uint8_t>(MouseButtonStatus::Release)) = true;
            break;

        case GLFW_PRESS:
            keyStatuses.at(static_cast<uint8_t>(MouseButtonStatus::Press)) = true;
            keyStatuses.at(static_cast<uint8_t>(MouseButtonStatus::Down)) = true;
            break;

        default:
            break;
    }
}

void CoreInput::MouseCursorPos(GLFWwindow*, const double_t xpos, const double_t ypos)
{
    m_MousePos = Vector2(static_cast<float_t>(xpos), static_cast<float_t>(ypos));
}

void CoreInput::HandleJoyStickCallBack(int jid, int event)
{
    switch (event)
    {
        case GLFW_CONNECTED :
            m_GamePads[jid].isConnected = true;
        break;

        case GLFW_DISCONNECTED :
            m_GamePads[jid].isConnected = false;
        break;
        default:;
    }
}
void CoreInput::HandleGamePad()
{
    for (uint32_t i = 0; i < GamePadMax ; i++)
    {
        if (!m_GamePads[i].isConnected)
            continue;

        GLFWgamepadstate state;

        if (!glfwGetGamepadState(static_cast<int32_t>(i), &state))
            return;
        
        for (uint32_t k = 0; k < static_cast<uint32_t>(GamePadAxes::Count); k++)
        {
            if(state.axes[k] <= NullAnalogValue)
                continue;
            m_GamePads[i].m_AxesValue[k] = state.axes[k];
        }

        for (uint32_t k = 0; k < static_cast<uint32_t>(GamepadButton::Count) - 1; k++)
        {
            GameButtonStatuses& statuses = m_GamePadsButton[i].at(k);
            const bool_t isDown = statuses.at(static_cast<uint8_t>(GameButtonStatus::Down));
            const bool_t isPress = statuses.at(static_cast<uint8_t>(GameButtonStatus::Press));
            
            switch (state.buttons[k])
            {
                case GLFW_RELEASE :
                    if (isDown || isPress)
                    {
                        statuses.at(static_cast<uint8_t>(GameButtonStatus::Release)) = true;
                        statuses.at(static_cast<uint8_t>(GameButtonStatus::Down)) = false;
                        statuses.at(static_cast<uint8_t>(GameButtonStatus::Press)) = false;
                    }
                break;
                
                case GLFW_PRESS :
                    statuses.at(static_cast<uint8_t>(GameButtonStatus::Release)) = false;
                    statuses.at(static_cast<uint8_t>(GameButtonStatus::Down)) = true;
                    statuses.at(static_cast<uint8_t>(GameButtonStatus::Press)) = true;
                    
                break;
                
                default:
                    break;
            }
        }
    }
}

void CoreInput::ResetKey()
{

    GLFWwindow* const windowPtr = Window::GetHandle();

    glfwSetKeyCallback(windowPtr, HandleKeyboard);
    glfwSetMouseButtonCallback(windowPtr, HandleMouseButton);
    glfwSetCursorPosCallback(windowPtr, MouseCursorPos);

    KeyStatuses defaultKeys;
    defaultKeys.fill(false);
    m_Keyboard.fill(defaultKeys);

    MouseStatuses defaultMouseButtons;
    defaultMouseButtons.fill(false);
    m_Mouse.fill(defaultMouseButtons);
}

void CoreInput::CheckGamePadAtLaunch()
{
    for (int32_t i = 0; i < static_cast<int32_t>(GamePadMax); i++)
    {
        const int32_t present = glfwJoystickPresent(i);
        m_GamePads.at(i).isConnected = static_cast<bool_t>(present);
    }
}

void CoreInput::Reset()
{
    for (uint8_t i = 0; i < static_cast<uint8_t>(MouseButton::Count) - 1; i++)
    {
        MouseStatuses& statuses = m_Mouse.at(i);
        statuses.at(static_cast<uint8_t>(MouseButtonStatus::Press)) = false;
        statuses.at(static_cast<uint8_t>(MouseButtonStatus::Release)) = false;
    }

    for (uint16_t i = 0; i < static_cast<uint16_t>(Key::Count) - 1; i++)
    {
        KeyStatuses& statuses = m_Keyboard.at(i);
        statuses.at(static_cast<uint16_t>(KeyStatus::Press)) = false;
        statuses.at(static_cast<uint16_t>(KeyStatus::Release)) = false;
        statuses.at(static_cast<uint8_t>(KeyStatus::Repeat)) = false;
    }

    for (uint32_t i = 0; i < m_GamePadsButton.size(); i++)
    {
        for (uint32_t k = 0; k < m_GamePadsButton.at(i).size(); k++)
        {
            for (uint32_t j = 0; j < m_GamePadsButton.at(i).at(k).size(); j++)
            {
                m_GamePadsButton.at(i).at(k).at(static_cast<uint8_t>(GameButtonStatus::Press)) = false;
                m_GamePadsButton.at(i).at(k).at(static_cast<uint8_t>(GameButtonStatus::Release)) = false;
            }
        }
    }
}



void CoreInput::Initialize()
{
    GLFWwindow* const windowPtr = static_cast<GLFWwindow*>(Window::GetHandle());

    glfwSetKeyCallback(windowPtr, HandleKeyboard);
    glfwSetMouseButtonCallback(windowPtr, HandleMouseButton);
    glfwSetCursorPosCallback(windowPtr, MouseCursorPos);
    glfwSetJoystickCallback(HandleJoyStickCallBack);

    KeyStatuses defaultKeys;
    defaultKeys.fill(false);
    m_Keyboard.fill(defaultKeys);

    MouseStatuses defaultMouseButtons;
    defaultMouseButtons.fill(false);
    m_Mouse.fill(defaultMouseButtons);

    CheckGamePadAtLaunch();
}

