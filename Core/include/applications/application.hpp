#pragma once

#include "core.hpp"
#include "window.hpp"
#include "world.hpp"
#include "input\core_input.hpp"
#include "rendering/renderer.hpp"

BEGIN_XNOR_CORE
class Application
{
public:

    template<class T>
    static T* GetApplication();

    Window window;

    Renderer renderer;
    
    XNOR_ENGINE Application();

    XNOR_ENGINE virtual ~Application() = default;

    virtual void Update() = 0;
    
protected:
    XNOR_ENGINE static inline Application* m_ApplicationInstance = nullptr;
};

template <class T>
T* Application::GetApplication()
{
    static_assert(std::is_base_of<T,Application>::value,"T is not base on Application");
    
    return  reinterpret_cast<T*>(m_ApplicationInstance);
}

END_XNOR_CORE
