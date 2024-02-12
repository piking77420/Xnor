#include "rendering/opengl/opengl_renderer.hpp"

#include <glad/glad.h>

using namespace XnorCore;

OpenGlRenderer::OpenGlRenderer()
{
	gladLoadGL();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void OpenGlRenderer::SetClearColor(const Vector4& color) const
{
	glClearColor(color.x, color.y, color.z, color.w);
}

void OpenGlRenderer::ClearColorAndDepth()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
