#pragma once

#include "frame_buffer.hpp"
#include "rendering/camera.hpp"

BEGIN_XNOR_CORE

class RendererContext
{
public:
	const Camera* camera;
	const FrameBuffer* framebuffer;
	bool_t isEditor = false;

	[[nodiscard]]
	bool_t IsValid() const;
};

END_XNOR_CORE
