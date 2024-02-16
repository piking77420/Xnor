#pragma once

#include "core.hpp"
#include "resource/texture.hpp"

BEGIN_XNOR_CORE


class FrameBuffer
{
public:

	FrameBuffer();

	~FrameBuffer();

	void Attach(const std::vector<Texture*>& renderTargets);

private:
	uint32_t m_Id;
};

END_XNOR_CORE