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

const vec2i FrameBuffer::GetSize() const
{
	return m_FrameBufferSize;
}

void FrameBuffer::CreateAttachement(std::vector<RenderTarget>& renderTargets)
{
	for (size_t i = 0; i < renderTargets.size(); ++i)
	{
		RHI::AttachTexture2DToFrameBuffer(m_Id,i,renderTargets[i].texture->GetID());
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




