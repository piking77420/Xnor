#include "rendering/buffer/vao.hpp"

#include <glad/glad.h>

using namespace XnorCore;

void VAO::BindBuffer() const
{
    glBindVertexArray(m_Id);
}

void VAO::UnBindBuffer() const
{
    glBindVertexArray(0);
}

void VAO::Init()
{
    glGenVertexArrays(1, &m_Id);
}

VAO::~VAO()
{
    if (glIsBuffer(m_Id))
        glDeleteBuffers(1, &m_Id);
}
