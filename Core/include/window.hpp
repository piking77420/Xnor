#pragma once

#include <functional>
#include <GLFW/glfw3.h>

#include "core.hpp"
#include "Maths/vector2i.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE Window
{
public:

	static inline bool resizeFrameBuffer = false;
	
	Window();

	~Window();

	[[nodiscard]]
	static Vector2i GetSize();

	[[nodiscard]]
	static void* GetWindow();

	[[nodiscard]]
	bool ShouldClose() const;

	void PollEvents() const;
	
	void SetCurrentContext();

	void SetCurrentContext() const;

	[[nodiscard]]
	double GetTime() const;

private:
	static void GlfwResizeFramebuffer(GLFWwindow* window,int width,int height);

	
	static inline GLFWwindow* m_Window = nullptr;
	static inline Vector2i m_Size = { 1280, 720 };
};
	
END_XNOR_CORE
