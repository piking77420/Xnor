#pragma once

#include "frame_buffer.hpp"
#include "rendering/camera.hpp"

/// @file renderer_context.hpp
/// @brief Defines the RendererContext class.

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
