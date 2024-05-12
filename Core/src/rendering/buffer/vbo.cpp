#include "rendering/buffer/vbo.hpp"

#include <glad/glad.h>

#include "rendering/rhi.hpp"
#include "rendering/buffer/vao.hpp"

using namespace XnorCore;

VBO::~VBO()
{
    if (glIsBuffer(m_Id))
    glDeleteBuffers(1, &m_Id);
}

void VBO::  Allocate(const size_t size, const void* const data , const BufferUsage bufferUsage)
{
    glNamedBufferData(m_Id, static_cast<uint32_t>(size), data, Rhi::BufferUsageToOpenglUsage(bufferUsage));
    
}

void VBO::UpdateData(const size_t offset, const size_t size, const void* const data)
{
    glNamedBufferSubData(m_Id, offset, size, data);
}


void VBO::BindBuffer() const
{
    glBindBuffer(GL_ARRAY_BUFFER,m_Id);
}

void VBO::UnBind() const
{
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

uint32_t VBO::GetId() const
{
    return m_Id;
}

void VBO::Init()
{
    glCreateBuffers(1, &m_Id);
}


