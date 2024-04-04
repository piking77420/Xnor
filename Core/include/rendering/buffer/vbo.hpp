#pragma once

#include "core.hpp"

BEGIN_XNOR_CORE

class VBO
{
public:

    VBO();
    
    ~VBO();

    DEFAULT_COPY_MOVE_OPERATIONS(VBO)

    void Allocate(const size_t size, const void* const data = nullptr);

    

private:
    uint32_t m_Id = 0;
    
};

END_XNOR_CORE