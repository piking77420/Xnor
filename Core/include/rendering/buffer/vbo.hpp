#pragma once

#include "core.hpp"
#include "rendering/rhi_typedef.hpp"

BEGIN_XNOR_CORE
class XNOR_ENGINE VBO
{
public:

    VBO();
    
    ~VBO();

    DEFAULT_COPY_MOVE_OPERATIONS(VBO)
    
    void Allocate(const size_t size, const void* const data = nullptr);
    
    void ComputeDivisor(const VBODescriptor& vboDescriptor);
    
private:
    uint32_t m_Id = 0;
    
};

END_XNOR_CORE