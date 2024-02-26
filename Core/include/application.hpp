#pragma once

#include "core.hpp"
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

    virtual void OnWindowRezize() = 0;
    
protected:
    XNOR_ENGINE static inline Application* m_ApplicationInstance = nullptr;
};

END_XNOR_CORE
