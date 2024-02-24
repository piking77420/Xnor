#include "window.hpp"

#include "utils/logger.hpp"

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

void Window::SetIcon(Texture& icon)
{
	if (icon.GetChannels() != 4)
	{
		Logger::LogError("Invalid texture for window icon {}", icon.GetName());
		return;
	}
	
	const Vector2i size = icon.GetSize();

	const GLFWimage image
	{
		.width = size.x,
		.height = size.y,
		.pixels = icon.GetData<uint8_t>()
	};
	
	glfwSetWindowIcon(m_Window, 1, &image);
}

void Window::GlfwResizeFramebuffer(GLFWwindow* window, int widht, int height)
{
	resizeFrameBuffer = true;
}
