#pragma once

#include <vector>

#include "core.hpp"
#include "rhi_typedef.hpp"

BEGIN_XNOR_CORE

class RenderPass
{
public:
    std::vector<RenderTargetInfo> renderPassAttachments;

    XNOR_ENGINE RenderPass() = default;
    
    XNOR_ENGINE ~RenderPass() = default;
    
    XNOR_ENGINE explicit RenderPass(const std::vector<RenderTargetInfo>& attachments);
};

END_XNOR_CORE
