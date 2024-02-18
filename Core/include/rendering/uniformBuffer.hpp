#pragma once

#include "core.hpp"

BEGIN_XNOR_CORE

class UniformBuffer
{
public:

    UniformBuffer();
    
    ~UniformBuffer();

    void Allocate(size_t size,const void* data);

    void Update(size_t size, size_t offset, const void* data);

    void Bind(uint32_t index);
    
private:
    uint32_t m_Id;
};


END_XNOR_CORE