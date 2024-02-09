#include "opengl_renderer.hpp"

#include <glad/glad.h>

OpenglRenderer::OpenglRenderer()
{
	gladLoadGL();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}


void OpenglRenderer::SetClearColor(const Vector4& color)
{
	glClearColor(color.x, color.y, color.z, color.w);
}

void OpenglRenderer::ClearColorAndDepth()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
