#pragma once

#include "core.hpp"

BEGIN_XNOR_CORE

class UniformBuffer
{
public:
    UniformBuffer();
    
    ~UniformBuffer();

    DEFAULT_COPY_MOVE_OPERATIONS(UniformBuffer)

    void Allocate(size_t size,const void* data) const;

    void Update(size_t size, size_t offset, const void* data) const;

    void Bind(uint32_t index) const;
    
private:
    uint32_t m_Id;
};

END_XNOR_CORE
