#pragma once

#include "frame_buffer.hpp"
#include "rendering/camera.hpp"

/// @file renderer_context.hpp
/// @brief Defines the RendererContext class.

BEGIN_XNOR_CORE

class RendererContext
{
public:
	const Camera* camera = nullptr;
	const FrameBuffer* framebuffer = nullptr;
	bool_t isEditor = false;

	[[nodiscard]]
	XNOR_ENGINE bool_t IsValid() const;
};

END_XNOR_CORE
