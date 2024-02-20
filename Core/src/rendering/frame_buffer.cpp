#include "rendering/frame_buffer.hpp"

#include "rendering/rhi.hpp"

using namespace XnorCore;


void FrameBuffer::Create(const RenderPass& renderPass,const vec2i frameBufferSize)
{
	m_FrameBufferSize = frameBufferSize;
	RHI::CreateFrameBuffer(&m_Id,renderPass.GetId());
}

void FrameBuffer::Destroy()
{
	RHI::DestroyFrameBuffer(&m_Id);
}

const Vector2i FrameBuffer::GetSize() const
{
	return m_FrameBufferSize;
}

void FrameBuffer::AttachColorAttachement(const std::vector<RenderTarget>& renderTargets)
{
	for (uint32_t i = 0; i < static_cast<uint32_t>(renderTargets.size()); i++)
	{
		RHI::AttachColorAttachementToFrameBuffer(m_Id, i, renderTargets[i].texture->GetId());
	}
	
}

void FrameBuffer::BindFrameBuffer()
{
	RHI::BindFrameBuffer(m_Id);
}

void FrameBuffer::UnBindFrameBuffer()
{
	RHI::BindFrameBuffer(0);
}




