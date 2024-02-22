#pragma once

#include <Maths/vector2i.hpp>

#include "core.hpp"
#include "window.hpp"

BEGIN_XNOR_CORE

class Screen
{
    STATIC_CLASS(Screen)
    
public:
    XNOR_ENGINE static Vector2i Size();

    XNOR_ENGINE static int32_t RefreshRate();

    static void Initialize();

private:
    static inline GLFWmonitor* m_Monitor = nullptr;
    static inline const GLFWvidmode* m_VideoMode = nullptr;
};

END_XNOR_CORE
