#include "rendering/buffer/vbo.hpp"

#include <glad/glad.h>

using namespace XnorCore;

void VBO::Allocate(const size_t size, const void* const data)
{
    glGenBuffers(1, &m_Id);
    glNamedBufferData(GL_ARRAY_BUFFER,size,data, GL_STATIC_DRAW);
}

