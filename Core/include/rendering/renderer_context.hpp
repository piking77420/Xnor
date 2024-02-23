#pragma once

#include "frame_buffer.hpp"
#include "rendering/camera.hpp"

BEGIN_XNOR_CORE

class RendererContext
{
public:
	const Camera* camera;
	const FrameBuffer* framebuffer;
	const RenderPass* renderPass;
	bool isEditor = false;

	bool IsValid();


};

END_XNOR_CORE
