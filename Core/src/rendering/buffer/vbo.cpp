#include "rendering/buffer/vbo.hpp"

#include <glad/glad.h>

#include "rendering/rhi.hpp"

using namespace XnorCore;

VBO::VBO()
{
    glGenBuffers(1, &m_Id);
}

VBO::~VBO()
{
    if (glIsBuffer(m_Id))
    glDeleteBuffers(1, &m_Id);
}

void VBO::Allocate(const size_t size, const void* const data)
{
    glGenBuffers(1, &m_Id);
    glNamedBufferData(GL_ARRAY_BUFFER,size,data, GL_STATIC_DRAW);
}

void VBO::ComputeDivisor(const VBODescriptor& vbodescriptor)
{

    for (size_t i = 0; i < vbodescriptor.vboAttributePointerSize; i++)
    {
        const VertexAttributePointer& vertexAttributePointer = vbodescriptor.vertexAttributesPointer[i];
        glEnableVertexArrayAttrib(m_Id, vertexAttributePointer.index);
        glVertexAttribPointer(vertexAttributePointer.index, static_cast<int32_t>(vertexAttributePointer.size), Rhi::GetOpenglDataType(vertexAttributePointer.bufferDatatype), vertexAttributePointer.normalized,
                              static_cast<GLsizei>(vertexAttributePointer.stride), vertexAttributePointer.pointer);
    }

    for (size_t i = 0; i < vbodescriptor.attributesDivisorPointerSize; i++)
    {
        const AttributeDivisor& attributeDivisor = vbodescriptor.attributesDivisorPointer[i];
        glVertexAttribDivisor(attributeDivisor.index, attributeDivisor.divisor);
    }
    
}

