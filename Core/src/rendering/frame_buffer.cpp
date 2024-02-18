#include "rendering/frame_buffer.hpp"

#include "rendering/rhi.hpp"

using namespace XnorCore;


FrameBuffer::FrameBuffer()
{
	RHI::CreateFrameBuffer(&m_Id);
}

FrameBuffer::~FrameBuffer()
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
		RHI::AttachColorAttachementToFrameBuffer(m_Id, i, renderTargets[i].texture->GetID());
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




