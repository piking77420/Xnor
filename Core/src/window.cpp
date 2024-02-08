#include "window.hpp"

Window::Window()
{
	glfwInit();

	m_Window = glfwCreateWindow(m_Size.x, m_Size.y, "XNOR Engine", nullptr, nullptr);

	glfwMakeContextCurrent(m_Window);

	glfwSwapInterval(1); // Enable vsync
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

Window::~Window()
{
	glfwDestroyWindow(m_Window);

	glfwTerminate();
}

Vector2i Window::GetSize() const
{
	return m_Size;
}

GLFWwindow* Window::GetWindow()
{
	return m_Window;
}

const GLFWwindow* Window::GetWindow() const
{
	return m_Window;
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(m_Window);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(m_Window);
}

void Window::SetCurrentContext()
{
	glfwMakeContextCurrent(m_Window);
}

void Window::SetCurrentContext() const
{
	glfwMakeContextCurrent(m_Window);
}
