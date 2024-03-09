#include "rendering/frame_buffer.hpp"
#include "rendering/render_pass.hpp"
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

uint32_t FrameBuffer::GetId() const
{
	return m_Id;
}
