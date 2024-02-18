#include "rendering/frame_buffer.hpp"

#include "rendering/rhi.hpp"

using namespace XnorCore;


FrameBuffer::FrameBuffer()
{
	RHI::CreateFrameBuffer(&m_Id);
}

FrameBuffer::~FrameBuffer()
{
	RHI::DestroyFrameBuffer(&m_Id);
}

void FrameBuffer::CreateAttachement(std::vector<RenderTarget>& renderTargets)
{
	for (RenderTarget& renderTarget : renderTargets)
	{
		TextureCreateInfo textureCreateInfo
		{
			nullptr,
			static_cast<uint32_t>(m_FrameBufferSize.x),
			static_cast<uint32_t>(m_FrameBufferSize.y),
			TextureFiltering::LINEAR,
			TextureWrapping::REPEAT,
			TextureFormat::RGB_16
		};
		
		switch (renderTarget.attachementHandle)
		{
		case AttachementHandle::COLOR :
			textureCreateInfo.textureFormat = TextureFormat::RGB_16F;
			break;
		case AttachementHandle::POSITION :
			textureCreateInfo.textureFormat = TextureFormat::RGB_16F;
			break;
		case AttachementHandle::NORMAL :
			textureCreateInfo.textureFormat = TextureFormat::RGB_16F;
			break;
		case AttachementHandle::TEXTURECOORD :
			textureCreateInfo.textureFormat = TextureFormat::RG_16F;
			break;
		}

		renderTarget.texture = new Texture(textureCreateInfo);
	}
}




