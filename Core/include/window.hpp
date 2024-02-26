#pragma once

#include <functional>
#include <GLFW/glfw3.h>

#include "core.hpp"
#include "Maths/vector2i.hpp"
#include "resource/texture.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE Window
{
	STATIC_CLASS(Window)
	
public:
	static inline bool resizeFrameBuffer = false;

	static void Initialize();

	static void Shutdown();

	[[nodiscard]]
	static Vector2i GetSize();

	[[nodiscard]]
	static bool ShouldClose();

	static void PollEvents();
	
	static void MakeContextCurrent();

	[[nodiscard]]
	static double GetTime();

	[[nodiscard]]
	static GLFWwindow* GetHandle();

	static void SetIcon(Texture& icon);

	static void HideCursor(bool value);

private:
	static void GlfwResizeFramebuffer(GLFWwindow* window, int width, int height);
	
	static inline GLFWwindow* m_Window = nullptr;
	static inline Vector2i m_Size = { 1280, 720 };
};
	
END_XNOR_CORE
