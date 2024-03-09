#pragma once

#include <vector>

#include "core.hpp"
#include "rhi_typedef.hpp"

/// @file render_pass.hpp
/// @brief Defines the XnorCore::RenderPass class

BEGIN_XNOR_CORE

/// @brief Represents a render pass, which is a std::vector of @ref RenderTargetInfo
class RenderPass
{
public:
    /// @brief Attachments
    std::vector<RenderTargetInfo> renderPassAttachments;

    XNOR_ENGINE RenderPass() = default;

    /// @brief Creates a render pass with attachments
    /// @param attachments Attachments
    XNOR_ENGINE explicit RenderPass(const std::vector<RenderTargetInfo>& attachments);

    XNOR_ENGINE void BeginRenderPass(const RenderPassBeginInfo& beginInfo) const;

    XNOR_ENGINE void EndRenderPass() const;
    
    XNOR_ENGINE ~RenderPass() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(RenderPass)
};

END_XNOR_CORE
