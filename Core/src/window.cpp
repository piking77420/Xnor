#include "window.hpp"
#include "input/core_input.hpp"

#include "Maths/vector3.hpp"

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

Vector2i Window::GetSize()
{
	return m_Size;
}

void* Window::GetWindow()
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

GLFWwindow* Window::GetHandle()
{
	return m_Window;
}

void Window::GlfwResizeFramebuffer([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] int width, [[maybe_unused]] int height)
{
	resizeFrameBuffer = true;
}
