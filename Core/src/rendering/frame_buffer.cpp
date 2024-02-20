#include "rendering/frame_buffer.hpp"

#include "rendering/rhi.hpp"

using namespace XnorCore;


FrameBuffer::FrameBuffer(const vec2i size) : m_FrameBufferSize(size)
{
	
}

void FrameBuffer::Create(const RenderPass& renderPass ,const  std::vector<Texture*>& textures)
{
	RHI::CreateFrameBuffer(&m_Id,renderPass.GetId(),textures);
}

void FrameBuffer::Destroy()
{
	RHI::DestroyFrameBuffer(&m_Id);
}

const Vector2i FrameBuffer::GetSize() const
{
	return m_FrameBufferSize;
}



void FrameBuffer::BindFrameBuffer()
{
	RHI::BindFrameBuffer(m_Id);
}

void FrameBuffer::UnBindFrameBuffer()
{
	RHI::BindFrameBuffer(0);
}




