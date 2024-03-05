#include "rendering/frame_buffer.hpp"

#include "rendering/rhi.hpp"

using namespace XnorCore;


FrameBuffer::FrameBuffer(const vec2i size)
	: m_FrameBufferSize(size), m_Id(0)
{
}

FrameBuffer::~FrameBuffer()
{
	Rhi::DestroyFrameBuffer(m_Id);
}

void FrameBuffer::Create(const RenderPass& renderPass, const std::vector<const Texture*>& attachements)
{
	m_Id = Rhi::CreateFrameBuffer(renderPass, attachements);
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

void FrameBuffer::ReadPixel(const uint32_t attachmentIndex, const Vector2i position, const TextureFormat textureFormat, const TextureInternalFormat textureInternalFormat, void* const output) const
{
	BindFrameBuffer();
	Rhi::GetPixelFromAttachement(attachmentIndex, position, textureFormat, textureInternalFormat, output);
	UnBindFrameBuffer();
}

uint32_t FrameBuffer::GetId() const
{
	return m_Id;
}




