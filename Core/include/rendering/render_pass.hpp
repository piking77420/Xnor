#pragma once

#include <vector>

#include "core.hpp"
#include "rhi_typedef.hpp"

BEGIN_XNOR_CORE

class RenderPass
{
public:
    
    RenderPass() = default;
    
    RenderPass(std::vector<AttachementsType> renderPass);

    ~RenderPass() = default;
private:
    uint32_t m_Id = 0;
};
END_XNOR_CORE
