#include  "rendering/render_pass.hpp"
#include "rendering/rhi.hpp"

using namespace XnorCore;


RenderPass::RenderPass(const std::vector<AttachementsType>& renderPass) : m_RenderPass(renderPass)
{
     RHI::CreateRenderPass(&m_Id,m_RenderPass);
}

uint32_t RenderPass::GetId() const
{
     return m_Id;
}
