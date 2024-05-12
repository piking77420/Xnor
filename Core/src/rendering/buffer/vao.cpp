#include "rendering/buffer/vao.hpp"

#include <glad/glad.h>

#include "rendering/rhi.hpp"

using namespace XnorCore;

void VAO::BindBuffer() const
{
    glBindVertexArray(m_Id);
}

void VAO::UnBindBuffer() const
{
    glBindVertexArray(0);
}

uint32_t VAO::GetId() const
{
    return m_Id;
}

void VAO::ComputeDescriptor(const VAODescriptor& vaoDescriptor) const
{
    for (size_t i = 0; i < vaoDescriptor.VertexAttributeBindingSize; i++)
    {
        const VertexAttributeBinding& vertexAttributeBinding = vaoDescriptor.vertexAttributeBindings[i];

        const VertexAttribFormat& vertexAttribFormats = vaoDescriptor.vertexAttribFormats[i];
        glEnableVertexArrayAttrib(m_Id,static_cast<uint32_t>(i));
        glVertexArrayAttribBinding(m_Id, vertexAttributeBinding.attribindex, vertexAttributeBinding.bindingindex);
        glVertexArrayAttribFormat(m_Id, vertexAttribFormats.attribindex, static_cast<int32_t>(vertexAttribFormats.size), Rhi::GetOpenglDataType(vertexAttribFormats.type), vertexAttribFormats.normalized, vertexAttribFormats.relativeoffset);
    }
    
    glVertexArrayVertexBuffer(m_Id, 0, vaoDescriptor.vboId, 0, vaoDescriptor.vertexAttribFormats[0].size * 4);


}

void VAO::Init()
{
    glCreateVertexArrays(1, &m_Id);
}

VAO::~VAO()
{
    if (glIsBuffer(m_Id))
        glDeleteBuffers(1, &m_Id);
}
