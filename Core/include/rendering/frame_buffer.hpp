#pragma once

#include "core.hpp"
#include "resource/texture.hpp"

BEGIN_XNOR_CORE


struct RenderTarget
{
	Texture* texture;
	Attachements attachementHandle;
};

class FrameBuffer
{
public:
	XNOR_ENGINE FrameBuffer();
	
	XNOR_ENGINE ~FrameBuffer();

	XNOR_ENGINE	const Vector2i GetSize() const;
	
	XNOR_ENGINE void AttachColorAttachement(const std::vector<RenderTarget>& renderTargets);
	
	XNOR_ENGINE void BindFrameBuffer();

	XNOR_ENGINE void UnBindFrameBuffer();
	
private:
	Vector2i m_FrameBufferSize = {1920,1080};
	uint32_t m_Id;
};

END_XNOR_CORE