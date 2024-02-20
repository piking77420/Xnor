#pragma once

#include "core.hpp"
#include "render_pass.hpp"
#include "resource/texture.hpp"

BEGIN_XNOR_CORE


class FrameBuffer
{
public:
	XNOR_ENGINE FrameBuffer() = default;
	
	XNOR_ENGINE FrameBuffer(const vec2i size);

	XNOR_ENGINE ~FrameBuffer() = default;
	
	DEFAULT_COPY_MOVE_OPERATIONS(FrameBuffer)
	
	XNOR_ENGINE void Create(const RenderPass& renderPass ,const std::vector<Texture*>& textures);

	XNOR_ENGINE void Destroy();
	
	XNOR_ENGINE	const Vector2i GetSize() const;
	
	XNOR_ENGINE void BindFrameBuffer();

	XNOR_ENGINE void UnBindFrameBuffer();
	
private:
	Vector2i m_FrameBufferSize = { 1920, 1080 };
	uint32_t m_Id;
};

END_XNOR_CORE
