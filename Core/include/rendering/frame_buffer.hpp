#pragma once

#include "core.hpp"
#include "window.hpp"
#include "resource/cubemap.hpp"
#include "resource/texture.hpp"

/// @file frame_buffer.hpp
/// @brief Defines the XnorCore::FrameBuffer class

BEGIN_XNOR_CORE
class RenderPass;

/// @brief Encapsulates a framebuffer object
class FrameBuffer
{
public:
	XNOR_ENGINE FrameBuffer();

	/// @brief Creates a framebuffer with a specified size
	/// @param size Size
	XNOR_ENGINE explicit FrameBuffer(Vector2i size);

	XNOR_ENGINE ~FrameBuffer();

	DEFAULT_COPY_MOVE_OPERATIONS(FrameBuffer)

	/// @brief Creates the framebuffer with a render pass and texture attachments
	/// @param renderPass Render pass
	/// @param attachments Texture attachments
	XNOR_ENGINE void AttachTextures(const RenderPass& renderPass, const std::vector<const Texture*>& attachments) const;

	XNOR_ENGINE void AttachTexture(const Texture& texture, Attachment::Attachment attachment, uint32_t level = 0) const;

	XNOR_ENGINE void AttachTexture(const Cubemap& cubemap, Attachment::Attachment attachment, CubeMapFace cubeMapFace, uint32_t level = 0) const;
	
	XNOR_ENGINE void GetPixelFromAttachment(uint32_t attachmentIndex, Vector2i position,
		TextureFormat::TextureFormat textureFormat, DataType::DataType dataType, void* output) const;

	/// @brief Gets the size of the framebuffer
	/// @return Size
	XNOR_ENGINE	Vector2i GetSize() const;
	
	/// @brief Gets the id of the framebuffer
	/// @return Framebuffer id
	XNOR_ENGINE uint32_t GetId() const;
	
private:
	Vector2i m_FrameBufferSize = Window::GetSize();
	uint32_t m_Id = 0;
};

END_XNOR_CORE
