#pragma once

#include "core.hpp"
#include "render_pass.hpp"
#include "window.hpp"
#include "resource/texture.hpp"

BEGIN_XNOR_CORE

class FrameBuffer
{
public:
	XNOR_ENGINE FrameBuffer() = default;
	
	XNOR_ENGINE explicit FrameBuffer(Vector2i size);

	XNOR_ENGINE ~FrameBuffer();
	
	DEFAULT_COPY_MOVE_OPERATIONS(FrameBuffer)
	
	XNOR_ENGINE void Create(const RenderPass& renderPass, const std::vector<const Texture*>& attachements);
	
	XNOR_ENGINE	Vector2i GetSize() const;
	
	XNOR_ENGINE void BindFrameBuffer() const;

	XNOR_ENGINE void UnBindFrameBuffer() const;

	XNOR_ENGINE void ReadPixel(uint32_t attachmentIndex,vec2i position,TextureFormat textureFormat,TextureInternalFormat textureInternalFormat,void* output) const;

	XNOR_ENGINE uint32_t GetId() const;
	
private:
	Vector2i m_FrameBufferSize = Window::GetSize();
	uint32_t m_Id;
};

END_XNOR_CORE
