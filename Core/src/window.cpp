#include "window.hpp"

XnorCore::Window::Window()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	m_Window = glfwCreateWindow(m_Size.x, m_Size.y, "XNOR Engine", nullptr, nullptr);

	glfwMakeContextCurrent(m_Window);

	glfwSwapInterval(1); // Enable vsync
}

XnorCore::Window::~Window()
{
	glfwDestroyWindow(m_Window);

	glfwTerminate();
}

Vector2i XnorCore::Window::GetSize() const
{
	return m_Size;
}

GLFWwindow* XnorCore::Window::GetWindow()
{
	return m_Window;
}

const GLFWwindow* XnorCore::Window::GetWindow() const
{
	return m_Window;
}

bool XnorCore::Window::ShouldClose() const
{
	return glfwWindowShouldClose(m_Window);
}

void XnorCore::Window::PollEvents()
{
	glfwPollEvents();
}

void XnorCore::Window::SwapBuffers() const
{
	glfwSwapBuffers(m_Window);
}

void XnorCore::Window::SetCurrentContext()
{
	glfwMakeContextCurrent(m_Window);
}

void XnorCore::Window::SetCurrentContext() const
{
	glfwMakeContextCurrent(m_Window);
}
