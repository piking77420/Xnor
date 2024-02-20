#pragma once

#include <vector>

#include "core.hpp"
#include "rhi_typedef.hpp"

BEGIN_XNOR_CORE

class RenderPass
{
public:
    
    XNOR_ENGINE RenderPass() = default;
    
    XNOR_ENGINE ~RenderPass() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(RenderPass)
    
    XNOR_ENGINE RenderPass(const std::vector<AttachementsType>& renderPass);

    XNOR_ENGINE uint32_t GetId() const;
  
    
private:
    uint32_t m_Id = 0;
    std::vector<AttachementsType> m_RenderPass;
};
END_XNOR_CORE
