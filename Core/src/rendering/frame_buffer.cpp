#include "rendering/frame_buffer.hpp"

#include "rendering/rhi.hpp"

using namespace XnorCore;


FrameBuffer::FrameBuffer(const vec2i size) : m_FrameBufferSize(size)
{
	
}

void FrameBuffer::Create(const RenderPass& renderPass,const std::vector<const Texture*>& textures)
{
	RHI::CreateFrameBuffer(&m_Id,renderPass,textures);
}

void FrameBuffer::Destroy()
{
	RHI::DestroyFrameBuffer(&m_Id);
}

const Vector2i FrameBuffer::GetSize() const
{
	return m_FrameBufferSize;
}



void FrameBuffer::BindFrameBuffer() const 
{
	RHI::BindFrameBuffer(m_Id);
}

void FrameBuffer::UnBindFrameBuffer() const 
{
	RHI::BindFrameBuffer(0);
}

uint32_t FrameBuffer::GetId() const
{
	return m_Id;
}




