#pragma once

#include <functional>
#include <GLFW/glfw3.h>

#include "core.hpp"
#include "Maths/vector2i.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE Window
{
public:
	Window();

	~Window();

	[[nodiscard]]
	static Vector2i GetSize();

	[[nodiscard]]
	void* GetWindow();

	[[nodiscard]]
	const void* GetWindow() const;

	[[nodiscard]]
	bool ShouldClose() const;

	void PollEvents() const;

	void SwapBuffers() const;

	void SetCurrentContext();

	void SetCurrentContext() const;

	[[nodiscard]]
	double GetTime() const;

	//std::vector<std::function<Vector2i>> OnFrameBufferResize;

private:
	static void GlfwResizeFramebuffer(GLFWwindow* window,int width,int height);

	
	static inline bool m_ResizeFrameBuffer = false;
	GLFWwindow* m_Window = nullptr;
	static inline Vector2i m_Size = { 1280, 720 };
};
	
END_XNOR_CORE
