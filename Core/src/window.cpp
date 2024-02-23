#include "window.hpp"

using namespace XnorCore;

void Window::Initialize()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	m_Window = glfwCreateWindow(m_Size.x, m_Size.y, "XNOR Engine", nullptr, nullptr);

	glfwSetFramebufferSizeCallback(m_Window, GlfwResizeFramebuffer);

	glfwMakeContextCurrent(m_Window);

	glfwSwapInterval(1); // Enable vsync
}

void Window::Shutdown()
{
	glfwDestroyWindow(m_Window);

	glfwTerminate();
}

Vector2i Window::GetSize()
{
	return m_Size;
}

void* Window::GetWindow()
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

void Window::MakeContextCurrent()
{
	glfwMakeContextCurrent(m_Window);
}

double Window::GetTime()
{
	return glfwGetTime();
}

GLFWwindow* Window::GetHandle()
{
	return m_Window;
}

void Window::GlfwResizeFramebuffer(GLFWwindow*, int, int)
{
	resizeFrameBuffer = true;
}
