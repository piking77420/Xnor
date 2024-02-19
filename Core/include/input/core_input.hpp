#pragma once

#include <queue>

#include "core.hpp"
#include "window.hpp"


BEGIN_XNOR_CORE
enum class KeyCode : uint32_t
{
       /* Printable keys */
    XNOR_KEY_SPACE              = 32,
    XNOR_KEY_APOSTROPHE         = 39  ,/* ' */
    XNOR_KEY_COMMA              = 44  ,/* , */
    XNOR_KEY_MINUS              = 45  ,/* - */
    XNOR_KEY_PERIOD             = 46  ,/* . */
    XNOR_KEY_SLASH              = 47  ,/* / */
    XNOR_KEY_0                  = 48,
    XNOR_KEY_1                  = 49,
    XNOR_KEY_2                  = 50,
    XNOR_KEY_3                  = 51,
    XNOR_KEY_4                  = 52,
    XNOR_KEY_5                  = 53,
    XNOR_KEY_6                  = 54,
    XNOR_KEY_7                  = 55,
    XNOR_KEY_8                  = 56,
    XNOR_KEY_9                  = 57,
    XNOR_KEY_SEMICOLON          = 59  ,/* ; */
    XNOR_KEY_EQUAL              = 61  ,/* = */
    XNOR_KEY_A                  = 65,
    XNOR_KEY_B                  = 66,
    XNOR_KEY_C                  = 67,
    XNOR_KEY_D                  = 68,
    XNOR_KEY_E                  = 69,
    XNOR_KEY_F                  = 70,
    XNOR_KEY_G                  = 71,
    XNOR_KEY_H                  = 72,
    XNOR_KEY_I                  = 73,
    XNOR_KEY_J                  = 74,
    XNOR_KEY_K                  = 75,
    XNOR_KEY_L                  = 76,
    XNOR_KEY_M                  = 77,
    XNOR_KEY_N                  = 78,
    XNOR_KEY_O                  = 79,
    XNOR_KEY_P                  = 80,
    XNOR_KEY_Q                  = 81,
    XNOR_KEY_R                  = 82,
    XNOR_KEY_S                  = 83,
    XNOR_KEY_T                  = 84,
    XNOR_KEY_U                  = 85,
    XNOR_KEY_V                  = 86,
    XNOR_KEY_W                  = 87,
    XNOR_KEY_X                  = 88,
    XNOR_KEY_Y                  = 89,
    XNOR_KEY_Z                  = 90,
    XNOR_KEY_LEFT_BRACKET       = 91  ,/* [ */
    XNOR_KEY_BACKSLASH          = 92  ,/* \ */
    XNOR_KEY_RIGHT_BRACKET      = 93  ,/* ] */
    XNOR_KEY_GRAVE_ACCENT       = 96  ,/* ` */
    XNOR_KEY_WORLD_1            = 161 ,/* non-US #1 */
    XNOR_KEY_WORLD_2            = 162 ,/* non-US #2 */

    /* Function keys */
    XNOR_KEY_ESCAPE             = 256,
    XNOR_KEY_ENTER              = 257,
    XNOR_KEY_TAB                = 258,
    XNOR_KEY_BACKSPACE          = 259,
    XNOR_KEY_INSERT             = 260,
    XNOR_KEY_DELETE             = 261,
    XNOR_KEY_RIGHT              = 262,
    XNOR_KEY_LEFT               = 263,
    XNOR_KEY_DOWN               = 264,
    XNOR_KEY_UP                 = 265,
    XNOR_KEY_PAGE_UP            = 266,
    XNOR_KEY_PAGE_DOWN          = 267,
    XNOR_KEY_HOME               = 268,
    XNOR_KEY_END                = 269,
    XNOR_KEY_CAPS_LOCK          = 280,
    XNOR_KEY_SCROLL_LOCK        = 281,
    XNOR_KEY_NUM_LOCK           = 282,
    XNOR_KEY_PRINT_SCREEN       = 283,
    XNOR_KEY_PAUSE              = 284,
    XNOR_KEY_F1                 = 290,
    XNOR_KEY_F2                 = 291,
    XNOR_KEY_F3                 = 292,
    XNOR_KEY_F4                 = 293,
    XNOR_KEY_F5                 = 294,
    XNOR_KEY_F6                 = 295,
    XNOR_KEY_F7                 = 296,
    XNOR_KEY_F8                 = 297,
    XNOR_KEY_F9                 = 298,
    XNOR_KEY_F10                = 299,
    XNOR_KEY_F11                = 300,
    XNOR_KEY_F12                = 301,
    XNOR_KEY_F13                = 302,
    XNOR_KEY_F14                = 303,
    XNOR_KEY_F15                = 304,
    XNOR_KEY_F16                = 305,
    XNOR_KEY_F17                = 306,
    XNOR_KEY_F18                = 307,
    XNOR_KEY_F19                = 308,
    XNOR_KEY_F20                = 309,
    XNOR_KEY_F21                = 310,
    XNOR_KEY_F22                = 311,
    XNOR_KEY_F23                = 312,
    XNOR_KEY_F24                = 313,
    XNOR_KEY_F25                = 314,
    XNOR_KEY_KP_0               = 320,
    XNOR_KEY_KP_1               = 321,
    XNOR_KEY_KP_2               = 322,
    XNOR_KEY_KP_3               = 323,
    XNOR_KEY_KP_4               = 324,
    XNOR_KEY_KP_5               = 325,
    XNOR_KEY_KP_6               = 326,
    XNOR_KEY_KP_7               = 327,
    XNOR_KEY_KP_8               = 328,
    XNOR_KEY_KP_9               = 329,
    XNOR_KEY_KP_DECIMAL         = 330,
    XNOR_KEY_KP_DIVIDE          = 331,
    XNOR_KEY_KP_MULTIPLY        = 332,
    XNOR_KEY_KP_SUBTRACT        = 333,
    XNOR_KEY_KP_ADD             = 334,
    XNOR_KEY_KP_ENTER           = 335,
    XNOR_KEY_KP_EQUAL           = 336,
    XNOR_KEY_LEFT_SHIFT         = 340,
    XNOR_KEY_LEFT_CONTROL       = 341,
    XNOR_KEY_LEFT_ALT           = 342,
    XNOR_KEY_LEFT_SUPER         = 343,
    XNOR_KEY_RIGHT_SHIFT        = 344,
    XNOR_KEY_RIGHT_CONTROL      = 345,
    XNOR_KEY_RIGHT_ALT          = 346,
    XNOR_KEY_RIGHT_SUPER        = 347,
    XNOR_KEY_MENU               = 348,
};

enum class KeyStatus : uint16_t
{
    DOWN,
    RELEASE,
    HOLD
};


class CoreInput
{
public:
    XNOR_ENGINE static bool GetKey(KeyCode keyCode, KeyStatus keyStatus = KeyStatus::DOWN);

private:
    struct InputKey
    {
        KeyCode keyCode;
        KeyStatus status;
    };

    XNOR_ENGINE static inline std::vector<InputKey>* m_InputQueue = nullptr;

public:
    XNOR_ENGINE CoreInput();
    
    XNOR_ENGINE ~CoreInput();

    XNOR_ENGINE void HandleInputsEvents(const Window& window);
    
private:
};

END_XNOR_CORE
