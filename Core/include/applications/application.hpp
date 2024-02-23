﻿#pragma once

#include "core.hpp"
#include "window.hpp"
#include "world.hpp"
#include "input\core_input.hpp"
#include "rendering/renderer.hpp"

BEGIN_XNOR_CORE

class Application
{
public:
    Renderer renderer;
    
    XNOR_ENGINE Application();

    XNOR_ENGINE virtual ~Application();

    DEFAULT_COPY_MOVE_OPERATIONS(Application)

    virtual void Update() = 0;
    
protected:
    XNOR_ENGINE static inline Application* m_ApplicationInstance = nullptr;
};

END_XNOR_CORE
