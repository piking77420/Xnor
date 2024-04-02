#include "rendering/frame_buffer.hpp"
#include "rendering/render_pass.hpp"
#include "rendering/rhi.hpp"

using namespace XnorCore;

FrameBuffer::FrameBuffer()
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

void FrameBuffer::AttachTexture(const Texture& texture, const Attachment::Attachment attachment, const uint32_t level) const
{
	Rhi::AttachTextureToFrameBuffer(m_Id, attachment, texture.GetId(), level);
}

void FrameBuffer::AttachTexture(const Cubemap& cubemap, const Attachment::Attachment attachment, const CubeMapFace cubeMapFace, const uint32_t level) const
{
	Rhi::AttachTextureToFrameBuffer(m_Id, attachment, cubeMapFace, cubemap.GetId(), level);
}

void FrameBuffer::AttachTextureLayer(const Texture& texture, Attachment::Attachment attachment,uint32_t level, uint32_t layer) const
{
	Rhi::AttachTextureToFrameBufferLayer(m_Id,attachment,texture.GetId(), level,layer);
}

void FrameBuffer::GetPixelFromAttachment(
	const uint32_t attachmentIndex,
	const Vector2i position,
	const TextureFormat::TextureFormat textureFormat,
	const DataType::DataType dataType,
	void* output
) const
{
	Rhi::BindFrameBuffer(m_Id);
	Rhi::GetPixelFromAttachement(attachmentIndex, position, textureFormat, dataType, output);
	Rhi::UnbindFrameBuffer();
}


uint32_t FrameBuffer::GetId() const
{
	return m_Id;
}
