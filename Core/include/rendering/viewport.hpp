#pragma once

#include "frame_buffer.hpp"
#include "viewport_data.hpp"
#include "rendering/camera.hpp"
#include "scene/entity.hpp"

/// @file renderer_context.hpp
/// @brief Defines the XnorCore::RendererContext class.

BEGIN_XNOR_CORE

/// @brief Class used to store everything needed to render a frame.
///
/// Stores the main @ref XnorCore::Viewport::camera "camera" along with the @ref XnorCore::Viewport::frameBuffer "frameBuffer".
class Viewport
{
public:
	
	/// @brief The Camera used to render a frame.
	const Camera* camera = nullptr;

	Vector2i viewPortSize = Window::GetSize();

	RenderPass colorPass;
	
	FrameBuffer* frameBuffer = nullptr;

	Texture* finalImage = nullptr;
	
	/// @brief Whether this is used in the editor.
	bool_t isEditor = false;
	
	ViewportData viewportData;

	// Returne true if there is a entities at this pixel
	XNOR_ENGINE bool GetEntityFromScreen(Vector2i pixelPos, XnorCore::Entity** entity) const;
	

	XNOR_ENGINE void Init(Vector2i newSize);

	XNOR_ENGINE void Destroy();

	XNOR_ENGINE void OnResize(Vector2i newSize);
	
	/// @brief Whether the context is valid.
	[[nodiscard]]
	XNOR_ENGINE bool_t IsValid() const;
};

END_XNOR_CORE
