#pragma once

#include "core.hpp"
#include "resource/texture.hpp"

BEGIN_XNOR_CORE


struct RenderTarget
{
	Texture* texture;
	AttachementHandle attachementHandle;
};

class FrameBuffer
{
public:

	FrameBuffer();
	
	~FrameBuffer();

	XNOR_ENGINE vec2i GetSize() const;
	
	XNOR_ENGINE void CreateAttachement(const std::vector<RenderTarget>& renderTargets);
	
	XNOR_ENGINE void BindFrameBuffer();

	XNOR_ENGINE void UnBindFrameBuffer();
	
private : 
	
	vec2i m_FrameBufferSize = {800,600};
	
	uint32_t m_Id;
};

END_XNOR_CORE