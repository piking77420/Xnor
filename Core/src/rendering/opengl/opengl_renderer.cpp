#include "opengl_renderer.hpp"

#include <glad/glad.h>

XnorCore::OpenGlRenderer::OpenGlRenderer()
{
	gladLoadGL();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void XnorCore::OpenGlRenderer::SetClearColor(const Vector4& color) const
{
	glClearColor(color.x, color.y, color.z, color.w);
}

void XnorCore::OpenGlRenderer::ClearColorAndDepth()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
