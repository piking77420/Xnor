﻿#pragma once

#include "core.hpp"

BEGIN_XNOR_CORE

class UniformBuffer
{
public:
    UniformBuffer();
    
    ~UniformBuffer();

    DEFAULT_COPY_MOVE_OPERATIONS_NO_ENGINE(UniformBuffer)

    void Allocate(size_t size,const void* data);

    void Update(size_t size, size_t offset, const void* data);

    void Bind(uint32_t index);
    
private:
    uint32_t m_Id;
};


END_XNOR_CORE
