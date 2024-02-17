#pragma once

#include "core.hpp"
#include "resource/texture.hpp"

BEGIN_XNOR_CORE
class FrameBuffer
{
public:
	struct RenderTarget
	{
		Texture* texture;
		AttachementHandle attachementHandle;
	};

	FrameBuffer();
	
	~FrameBuffer();

	void CreateAttachement(std::vector<RenderTarget>& renderTargets);
private:
	vec2i m_FrameBufferSize = {800,600};
	
	uint32_t m_Id;
};

END_XNOR_CORE