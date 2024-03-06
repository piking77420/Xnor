#include "rendering/frame_buffer.hpp"

#include "rendering/rhi.hpp"

using namespace XnorCore;


FrameBuffer::FrameBuffer(const Vector2i size)
	: m_FrameBufferSize(size)
{
}

FrameBuffer::~FrameBuffer()
{
	Rhi::DestroyFrameBuffer(m_Id);
}

void FrameBuffer::Create(const RenderPass& renderPass, const std::vector<const Texture*>& attachments)
{
	m_Id = Rhi::CreateFrameBuffer(renderPass, attachments);
}

Vector2i FrameBuffer::GetSize() const
{
	return m_FrameBufferSize;
}

void FrameBuffer::BindFrameBuffer() const 
{
	Rhi::BindFrameBuffer(m_Id);
}

void FrameBuffer::UnBindFrameBuffer() const 
{
	Rhi::BindFrameBuffer(0);
}

void FrameBuffer::ReadPixel(const uint32_t attachmentIndex, const Vector2i position, const TextureFormat format,
	const TextureInternalFormat internalFormat, void* const output) const
{
	BindFrameBuffer();
	Rhi::GetPixelFromAttachement(attachmentIndex, position, format, internalFormat, output);
	UnBindFrameBuffer();
}

uint32_t FrameBuffer::GetId() const
{
	return m_Id;
}




