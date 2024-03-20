#pragma once

#include "core.hpp"
#include "rendering/renderer.hpp"

/// @file application.hpp
/// @brief Defines the XnorCore::Application class.

BEGIN_XNOR_CORE

/// @brief Application interface, handles the lifetime of most of the sub systems needed for an XNOR application to run
class Application
{
public:
    /// @brief The path of the current executable
    ///
    /// This is valid once @ref Application::Application "the Application constructor" has been called
    static inline std::filesystem::path executablePath;
    
    /// @brief Renderer of the application
    Renderer renderer;

    /// @brief ctor, Initializes the sub systems
    XNOR_ENGINE Application();

    /// @brief dtor, Destroys the sub systems
    XNOR_ENGINE virtual ~Application();

    DEFAULT_COPY_MOVE_OPERATIONS(Application)

    /// @brief Main loop of an application
    XNOR_ENGINE virtual void Update() = 0;

    /// @brief Should handle the window resize actions
    XNOR_ENGINE virtual void CheckWindowResize() = 0;
    
protected:
    /// @brief Current Application instance.
    XNOR_ENGINE static inline Application* m_ApplicationInstance = nullptr;
};

END_XNOR_CORE
