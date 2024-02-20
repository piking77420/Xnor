#include "rendering/rhi.hpp"
#include "rendering\render_pass.hpp"

using namespace XnorCore;

 RenderPass::RenderPass(std::vector<AttachementsType> renderPass)
{
    RHI::CreateRenderPass(renderPass);
}
