#pragma once
#include "rendering/camera.hpp"
#include "frame_buffer.hpp"

BEGIN_XNOR_CORE

class RendererContext
{
public:
	Camera* camera = nullptr;
	FrameBuffer* framebuffer = nullptr;

	bool IsValid() const
	{
		return camera != nullptr && framebuffer != nullptr;
	}
};

END_XNOR_CORE