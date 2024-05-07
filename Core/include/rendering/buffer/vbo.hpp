#pragma once

#include "core.hpp"
#include "rendering/rhi_typedef.hpp"

BEGIN_XNOR_CORE
class XNOR_ENGINE VBO
{
public:

    VBO() = default;
    
    ~VBO();

    DEFAULT_COPY_MOVE_OPERATIONS(VBO)
    
    /// @brief Allocates a vertex buffer on the GPU
    /// @param size Data size
    /// @param data Data
    /// @param bufferUsage how the buffer will be use
    void Allocate(size_t size, const void* const data = nullptr, BufferUsage usage);

    void UpdateData(size_t offset, size_t size, const void* const data = nullptr);
    
    void ComputeDivisor(const VBODescriptor& vboDescriptor);

    void BindBuffer() const;

    void UnBind() const;

private:
    uint32_t m_Id = 0;
    
};

END_XNOR_CORE