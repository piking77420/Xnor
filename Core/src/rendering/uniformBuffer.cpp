#include "rendering/uniformBuffer.hpp"

#include <glad/glad.h>

using namespace XnorCore;

UniformBuffer::UniformBuffer()
{
    glCreateBuffers(1, &m_Id);
}

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &m_Id);
}

void UniformBuffer::Allocate(size_t size, const void* data)
{
    glNamedBufferStorage(m_Id, size, data, GL_DYNAMIC_STORAGE_BIT);
}

void UniformBuffer::Update(size_t size, size_t offset, const void* data)
{
    glNamedBufferSubData(m_Id, offset, size, data);   
}

void UniformBuffer::Bind(uint32_t index)
{
    glBindBufferBase(GL_UNIFORM_BUFFER, index, m_Id);
}
