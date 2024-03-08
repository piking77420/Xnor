#pragma once

#include "frame_buffer.hpp"
#include "rendering/camera.hpp"

/// @file renderer_context.hpp
/// @brief Defines the XnorCore::RendererContext class.

BEGIN_XNOR_CORE

/// @brief Class used to store everything needed to render a frame.
///
/// Stores the main @ref XnorCore::RendererContext::camera "camera" along with the @ref XnorCore::RendererContext::frameBuffer "frameBuffer".
class RendererContext
{
public:
	/// @brief The Camera used to render a frame.
	const Camera* camera = nullptr;
	/// @brief The FrameBuffer on which the frame should be rendered.
	const FrameBuffer* frameBuffer = nullptr;
	/// @brief Whether this is used in the editor.
	bool_t isEditor = false;

	/// @brief Whether the context is valid.
	[[nodiscard]]
	XNOR_ENGINE bool_t IsValid() const;
};

END_XNOR_CORE
