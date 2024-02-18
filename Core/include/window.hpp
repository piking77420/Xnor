#pragma once

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
	Vector2i GetSize() const;

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

private:
	GLFWwindow* m_Window = nullptr;
	Vector2i m_Size = { 1920,1080 };
};
	
END_XNOR_CORE
