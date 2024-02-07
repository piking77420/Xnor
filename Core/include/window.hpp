#pragma once

#include<GLFW/glfw3.h>

#include "core.hpp"
#include "Maths/vector2i.hpp"

class XNOR_ENGINE Window
{

public:

	Window();

	~Window();

	Vector2i GetSize() const
	{
		return m_Size;
	}

	Vector2i GetSize()
	{
		return m_Size;
	}

	GLFWwindow* GetWindow()
	{
		return m_Window;
	}

	const GLFWwindow* GetWindow() const 
	{
		return m_Window;
	}

	bool ShouldClose();

	void PoolEvents();

	void SwapBuffers();

	void SetCurrentContext();

	void SetCurrentContext() const ;


private:
	GLFWwindow* m_Window = nullptr;
	Vector2i m_Size = { 800,600 };
};

