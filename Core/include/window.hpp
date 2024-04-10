#pragma once

#include <functional>
#include <GLFW/glfw3.h>

#include "core.hpp"
#include "Maths/vector2i.hpp"
#include "resource/texture.hpp"

/// @file window.hpp
/// @brief Defines the XnorCore::Window class.

BEGIN_XNOR_CORE

/// @brief Options for the Window::MessageBox function.
BEGIN_ENUM(MessageBoxOptions)
{
	/// @brief The MessageBox contains the single option Ok.
	Ok,
	/// @brief The MessageBox contains the options Ok, Cancel.
	OkCancel,
	/// @brief The MessageBox contains the options Yes, No, Cancel.
	YesNoCancel,
	/// @brief The MessageBox contains the options Yes, No.
	YesNo,
	/// @brief The MessageBox contains the options Retry, Cancel.
	RetryCancel,
	/// @brief The MessageBox contains the options Cancel, Try Again, Continue.
	CancelTryContinue
}
END_ENUM

/// @brief Additional flags for the Window::MessageBox function.
BEGIN_ENUM(MessageBoxFlags)
{
	/// @brief No additional flags.
	None,

	/// @brief Shows an exclamation mark in a triangle.
	IconWarning,
	/// @brief Shows a stop sign.
	IconError,
	/// @brief Shows an <i>i</i> in a circle.
	IconInformation,

	/// @brief Sets the default button as the first one. This is the case by default.
	DefaultButton1,
	/// @brief Sets the default button as the second one.
	DefaultButton2,
	/// @brief Sets the default button as the third one.
	DefaultButton3,
	/// @brief Sets the default button as the fourth one.
	DefaultButton4
}
END_ENUM

/// @brief A wrapper for the main window
class XNOR_ENGINE Window
{
	STATIC_CLASS(Window)
	
public:
	/// @brief Whether the frame buffer was resized last frame
	static inline bool_t resizeFrameBuffer = false;

	/// @brief Initializes the window
	static void Initialize();

	/// @brief Terminates the window
	static void Shutdown();

	/// @brief Gets the size in pixels of the window
	/// @return Size
	[[nodiscard]]
	static Vector2i GetSize();

	/// @brief Gets whether the window should close
	/// @return Should close
	[[nodiscard]]
	static bool_t ShouldClose();

	/// @brief Polls the events of the window
	static void PollEvents();

	/// @brief Sets the window to be the current context
	static void MakeContextCurrent();
	
	/// @brief Gets the native handle of the window
	/// @return Native handle
	[[nodiscard]]
	static GLFWwindow* GetHandle();

	/// @brief Displays the window
	static void Show();

	/// @brief Sets the icon for the window
	/// @param icon Icon
	static void SetIcon(Texture& icon);

	/// @brief Handles hiding or displaying the cursor
	/// @param value @c true hides, @c false displays
	static void SetCursorHidden(bool_t value);

	/// @brief Sets the mouse cursor position.
	static void SetCursorPosition(Vector2 newPosition);

	static void MessageBox(const std::string& title, const std::string& text, ENUM_VALUE(MessageBoxOptions) flags);

private:
	/// @brief Callback when the window is resized
	/// @param window Native window handle
	/// @param width New width
	/// @param height New height
	static void GlfwResizeFramebuffer(GLFWwindow* window, int32_t width, int32_t height);

	/// @brief Native window handle
	static inline GLFWwindow* m_Window = nullptr;

	/// @brief Window size
	static inline Vector2i m_Size = { 1280, 720 };
};
	
END_XNOR_CORE
