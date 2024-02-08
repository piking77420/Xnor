#pragma once

#include <GLFW/glfw3.h>

#include "core.hpp"
#include "Maths/vector2i.hpp"

class XNOR_ENGINE Window
{
public:
	Window();

	~Window();

	[[nodiscard]]
	Vector2i GetSize() const;

	GLFWwindow* GetWindow();

	[[nodiscard]]
	const GLFWwindow* GetWindow() const;

	bool ShouldClose();

	void PollEvents();

	void SwapBuffers();

	void SetCurrentContext();

	void SetCurrentContext() const;

private:
	GLFWwindow* m_Window = nullptr;
	Vector2i m_Size = { 800,600 };
};
