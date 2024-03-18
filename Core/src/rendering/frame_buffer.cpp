#include "rendering/frame_buffer.hpp"
#include "rendering/render_pass.hpp"
#include "rendering/rhi.hpp"

using namespace XnorCore;

FrameBuffer::FrameBuffer()
{
	m_Id = Rhi::CreateFrameBuffer();
}

FrameBuffer::FrameBuffer(const Vector2i size)
	: m_FrameBufferSize(size)
{
	m_Id = Rhi::CreateFrameBuffer();
}

FrameBuffer::~FrameBuffer()
{
	Rhi::DestroyFrameBuffer(m_Id);
}

void FrameBuffer::AttachTextures(const RenderPass& renderPass, const std::vector<const Texture*>& attachments) const 
{
	Rhi::AttachsTextureToFrameBuffer(renderPass, *this, attachments);
}

void FrameBuffer::AttachTexture(const Texture& texture, const Attachment::Attachment attachment,const uint32_t level) const
{
	Rhi::AttachTextureToFrameBuffer(m_Id,attachment,texture.GetId(), level);
}

void FrameBuffer::AttachTexture(const Cubemap& cubemap, const Attachment::Attachment attachment, CubeMapFace cubeMapFace, const uint32_t level) const
{
	Rhi::AttachTextureToFrameBuffer(m_Id,attachment,cubeMapFace ,cubemap.GetId(), level);
}

void FrameBuffer::GetPixelFromAttachment(
	uint32_t attachmentIndex,
	Vector2i position,
	TextureFormat::TextureFormat textureFormat,
	DataType::DataType dataType,
	void* output
) const
{
	Rhi::BindFrameBuffer(m_Id);
	Rhi::GetPixelFromAttachement(attachmentIndex,position,textureFormat,dataType,output);
	Rhi::UnbindFrameBuffer();

}

Vector2i FrameBuffer::GetSize() const
{
	return m_FrameBufferSize;
}

uint32_t FrameBuffer::GetId() const
{
	return m_Id;
}
