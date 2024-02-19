#include "window.hpp"

using namespace XnorCore;

Window::Window()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	m_Window = glfwCreateWindow(m_Size.x, m_Size.y, "XNOR Engine", nullptr, nullptr);

	glfwSetFramebufferSizeCallback(m_Window,GlfwResizeFramebuffer);

	glfwMakeContextCurrent(m_Window);

	glfwSwapInterval(1); // Enable vsync
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

void* Window::GetWindow()
{
	return m_Window;
}

const void* Window::GetWindow() const
{
	return m_Window;
}

bool Window::ShouldClose() const
{
	return glfwWindowShouldClose(m_Window);
}

void Window::PollEvents() const
{
	glfwPollEvents();
	
}

void Window::SwapBuffers() const
{
	if(m_ResizeFrameBuffer)
	{
		// TO DO TRIGGER On Resize callBacks	
	}
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

double Window::GetTime() const
{
	return glfwGetTime();
}

void Window::GlfwResizeFramebuffer(GLFWwindow* window, [[maybe_unused]] int width, [[maybe_unused]] int height)
{
	m_ResizeFrameBuffer = true;
}
