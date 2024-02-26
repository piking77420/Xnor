#include  "rendering/render_pass.hpp"
#include "rendering/rhi.hpp"

using namespace XnorCore;


RenderPass::RenderPass(const std::vector<RenderTargetInfo>& attachments) : renderPassAttachments(attachments)
{
     
}
