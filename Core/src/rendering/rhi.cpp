#include "rendering/rhi.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.hpp"
#include "magic_enum/magic_enum.hpp"
#include "rendering/render_pass.hpp"
#include "rendering/frame_buffer.hpp"
#include "resource/shader.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void Rhi::SetPolygonMode(const PolygonFace::PolygonFace face, const PolygonMode::PolygonMode mode)
{
	glPolygonMode(static_cast<GLenum>(face), GL_POINT + static_cast<GLenum>(mode));
}

void Rhi::SetViewport(const Vector2i screenOffset, const Vector2i screenSize)
{
	glViewport(screenOffset.x, screenOffset.y, screenSize.x, screenSize.y);
}

void Rhi::DrawQuad(const uint32_t quadId)
{
	const ModelInternal model = m_ModelMap.at(quadId);
	glBindVertexArray(model.vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Rhi::BeginRenderPassInternal(const RenderPassBeginInfo& beginInfo)
{
	BindFrameBuffer(beginInfo.frameBuffer->GetId());
	SetClearColor(beginInfo.clearColor);
	
	if (beginInfo.clearBufferFlags != BufferFlag::None)
		ClearBuffer(beginInfo.clearBufferFlags);

	SetViewport(beginInfo.renderAreaOffset, beginInfo.renderAreaExtent);
}

void Rhi::EndRenderPass()
{
	UnbindFrameBuffer();
}

uint32_t Rhi::CreateModel(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	ModelInternal modelInternal;
	modelInternal.nbrOfVertex = static_cast<uint32_t>(vertices.size());
	modelInternal.nbrOfIndicies = static_cast<uint32_t>(indices.size()); 
	
	glCreateVertexArrays(1, &modelInternal.vao);

	glCreateBuffers(1, &modelInternal.vbo);
	glCreateBuffers(1, &modelInternal.ebo);

	GLintptr offset = static_cast<GLintptr>(vertices.size() * sizeof(Vertex));
	glNamedBufferData(modelInternal.vbo, offset, vertices.data(), GL_STATIC_DRAW);
	offset = static_cast<GLintptr>(indices.size() * sizeof(uint32_t));
	glNamedBufferData(modelInternal.ebo, offset, indices.data(), GL_STATIC_DRAW);

	// Position
	glEnableVertexArrayAttrib(modelInternal.vao, 0);
	glVertexArrayAttribBinding(modelInternal.vao, 0, 0);
	glVertexArrayAttribFormat(modelInternal.vao, 0, 3, GL_FLOAT, GL_FALSE, 0);

	// Normal
	glEnableVertexArrayAttrib(modelInternal.vao, 1);
	glVertexArrayAttribBinding(modelInternal.vao, 1, 0);
	glVertexArrayAttribFormat(modelInternal.vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));

	// Texture Coord
	glEnableVertexArrayAttrib(modelInternal.vao, 2);
	glVertexArrayAttribBinding(modelInternal.vao, 2, 0);
	glVertexArrayAttribFormat(modelInternal.vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, textureCoord));

	// Tangent
	glEnableVertexArrayAttrib(modelInternal.vao, 3);
	glVertexArrayAttribBinding(modelInternal.vao, 3, 0);
	glVertexArrayAttribFormat(modelInternal.vao, 3, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, tangent));
	
	// bitangent 
	glEnableVertexArrayAttrib(modelInternal.vao, 4);
	glVertexArrayAttribBinding(modelInternal.vao, 4, 0);
	glVertexArrayAttribFormat(modelInternal.vao, 4, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, bitangent));

	glEnableVertexArrayAttrib(modelInternal.vao, 5);
	glVertexArrayAttribBinding(modelInternal.vao, 5, 0);
	glVertexArrayAttribFormat(modelInternal.vao, 5, Vertex::MaxBoneWeight, GL_FLOAT, GL_FALSE, offsetof(Vertex, boneIndices));
	
	glEnableVertexArrayAttrib(modelInternal.vao, 6);
	glVertexArrayAttribBinding(modelInternal.vao, 6, 0);
	glVertexArrayAttribFormat(modelInternal.vao, 6, Vertex::MaxBoneWeight, GL_FLOAT, GL_FALSE, offsetof(Vertex,boneWeight));

	glVertexArrayVertexBuffer(modelInternal.vao, 0, modelInternal.vbo, 0, sizeof(Vertex));
	glVertexArrayElementBuffer(modelInternal.vao, modelInternal.ebo);
	
	const uint32_t modelId = modelInternal.vao;
	
	m_ModelMap.emplace(modelId, modelInternal);
	
	return modelId;
}

bool_t Rhi::DestroyModel(const uint32_t modelId)
{
	if (!m_ModelMap.contains(modelId))
		return false;

	const ModelInternal* model = &m_ModelMap.at(modelId);

	glDeleteBuffers(1, &model->vbo);
	glDeleteBuffers(1, &model->ebo);
	glDeleteVertexArrays(1, &model->vao);

	return true;
}

void Rhi::DrawModel(const uint32_t modelId)
{
	const ModelInternal model = m_ModelMap.at(modelId);
	glBindVertexArray(model.vao);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(model.nbrOfIndicies), GL_UNSIGNED_INT, nullptr);
}

void Rhi::DestroyProgram(const uint32_t shaderId)
{
	IsShaderValid(shaderId);
	glDeleteProgram(shaderId);
}

void Rhi::CheckCompilationError(const uint32_t shaderId, const std::string& type)
{
	int success;
	std::string infoLog(1024, '\0');

	if (type != "PROGRAM")
	{
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog.data());
			Logger::LogError("Error while compiling shader of type {}: {}", type, infoLog);
		}
	}
	else
	{
		glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderId, 1024, nullptr, infoLog.data());
			Logger::LogError("Error while linking shader program of type {}: {}", type, infoLog);
		}
	}
}

uint32_t Rhi::CreateShaders(const std::vector<ShaderCode>& shaderCodes, const ShaderCreateInfo& shaderCreateInfo)
{
	const uint32_t programId = glCreateProgram();
	std::vector<uint32_t> shaderIds(shaderCodes.size());

	for (size_t i = 0; i < shaderCodes.size(); i++)
	{
		const ShaderCode& code = shaderCodes[i];
		if (code.code.empty())
			continue;

		shaderIds[i] = glCreateShader(GetOpenglShaderType(code.type));
		const char_t* data = code.code.c_str();
		glShaderSource(shaderIds[i], 1, &data, &code.codeLength);
		glCompileShader(shaderIds[i]);

		CheckCompilationError(shaderIds[i], GetShaderTypeToString(code.type));
		glAttachShader(programId, shaderIds[i]);
		
	}
	glLinkProgram(programId);

	for (size_t i = 0; i < shaderIds.size(); i++)
	{
		if (glIsShader(shaderIds[i]))
			glDeleteShader(shaderIds[i]);
	}
		
	CheckCompilationError(programId, "PROGRAM");

	ShaderInternal shaderInternal;
	shaderInternal.depthFunction = shaderCreateInfo.depthFunction;
	shaderInternal.blendFunction = shaderCreateInfo.blendFunction;
	shaderInternal.cullInfo = shaderCreateInfo.shaderProgramCullInfo;
	
	m_ShaderMap.emplace(programId, shaderInternal);
	
	return programId;
}

void Rhi::UseShader(const uint32_t shaderId)
{
#ifdef _DEBUG
	IsShaderValid(shaderId);
#endif

	const ShaderInternal& shaderInternal = m_ShaderMap.at(shaderId);

	ComputeDepthFunction(shaderInternal.depthFunction);
	
	if (shaderInternal.blendFunction.isBlending)
	{
		const uint32_t srcValue = GetBlendValueOpengl(shaderInternal.blendFunction.sValue);
		const uint32_t destValue = GetBlendValueOpengl(shaderInternal.blendFunction.dValue);
		const uint32_t blendEquation = BlendEquationToOpengl(shaderInternal.blendFunction.blendEquation);
		glEnable(GL_BLEND);
		glBlendFunc(srcValue, destValue);
		glBlendEquation(blendEquation);
		m_Blending = true;
	}
	
	if (shaderInternal.cullInfo.enableCullFace)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(CullFaceToOpenglCullFace(shaderInternal.cullInfo.cullFace));
		glFrontFace(FrontFaceToOpenglFrontFace(shaderInternal.cullInfo.frontFace));
		m_Cullface = true;
	}
	
	glUseProgram(shaderId);
}

void Rhi::UnuseShader()
{
	if (m_Blending)
	{
		glDisable(GL_BLEND);
		m_Blending = false;
	}

	if (m_Cullface)
	{
		glDisable(GL_CULL_FACE);
		m_Cullface = false;
	}
	
	glUseProgram(0);
}

void Rhi::SetUniform(const UniformType::UniformType uniformType, const void* const data, const uint32_t shaderId, const char_t* const uniformKey)
{
	const GLint uniformLocation = GetUniformInMap(shaderId, uniformKey);

	switch (uniformType)
	{
		case UniformType::Int:
			glUniform1i(uniformLocation, *static_cast<const int32_t*>(data));
			break;
			
		case UniformType::Bool:
			glUniform1i(uniformLocation, *static_cast<const bool_t*>(data));
			break;
			
		case UniformType::Float:
			glUniform1f(uniformLocation, *static_cast<const GLfloat*>(data));
			break;

		case UniformType::Vec2:
			glUniform2fv(uniformLocation, 1, static_cast<const GLfloat*>(data));
			break;

		case UniformType::Vec3:
			glUniform3fv(uniformLocation, 1, static_cast<const GLfloat*>(data));
			break;
			
		case UniformType::Vec4:
			glUniform4fv(uniformLocation, 1, static_cast<const GLfloat*>(data));
			break;
			
		case UniformType::Mat3:
			glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, static_cast<const GLfloat*>(data));
			break;
			
		case UniformType::Mat4:
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, static_cast<const GLfloat*>(data));
			break;
	}
}

uint32_t Rhi::CreateTextureId(const TextureType::TextureType textureType)
{
	uint32_t textureId = 0;
	glCreateTextures(GetOpenglTextureType(textureType), 1, &textureId);
	return textureId;
}

uint32_t Rhi::GetOpenglTextureFilter(const TextureFiltering::TextureFiltering textureFiltering)
{
	switch (textureFiltering)
	{
		case TextureFiltering::None:
			return GL_NONE;
			
		case TextureFiltering::Linear:
			return GL_LINEAR;
			
		case TextureFiltering::Nearest:
			return GL_NEAREST;

		case TextureFiltering::NearestMipmapNearest:
		case TextureFiltering::LinearMimapNearest:
		case TextureFiltering::NearestMimapLinear:
		case TextureFiltering::LinearMimMapLinear:
			break;
	}

	return GL_LINEAR;
}

uint32_t Rhi::GetBlendValueOpengl(const BlendValue::BlendValue blendFunction)
{
	switch (blendFunction)
	{
		case BlendValue::Zero:
			return GL_ZERO;
			
		case BlendValue::One:
			return GL_ONE;
			
		case BlendValue::SrcColor:
			return GL_SRC_COLOR;
			
		case BlendValue::OneMinusSrcColor:
			return GL_ONE_MINUS_SRC_COLOR;
			
		case BlendValue::DstColor:
			return GL_DST_COLOR;
			
		case BlendValue::OneMinusDstColor:
			return GL_ONE_MINUS_DST_COLOR;
			
		case BlendValue::SrcAlpha:
			return GL_SRC_ALPHA;
			
		case BlendValue::OneMinusSrcAlpha:
			return GL_ONE_MINUS_SRC_ALPHA;
			
		case BlendValue::DstAlpha:
			return GL_DST_ALPHA;
			
		case BlendValue::OneMinusDstAlpha:
			return GL_ONE_MINUS_DST_ALPHA;
			
		case BlendValue::ConstantColor:
			return GL_CONSTANT_COLOR;
			
		case BlendValue::OneMinusConstantColor:
			return GL_ONE_MINUS_CONSTANT_COLOR;
			
		case BlendValue::ConstantAlpha:
			return GL_CONSTANT_ALPHA;
			
		case BlendValue::OneMinusConstantAlpha:
			return GL_ONE_MINUS_CONSTANT_ALPHA;
	}

	return GL_ONE; 
}

uint32_t Rhi::GetOpenglBufferBit(const BufferFlag::BufferFlag flag)
{
	uint32_t openglBufferBit = 0;

	if (flag & BufferFlag::ColorBit)
		openglBufferBit |= GL_COLOR_BUFFER_BIT;

	if (flag & BufferFlag::DepthBit)
		openglBufferBit |= GL_DEPTH_BUFFER_BIT;

	if (flag & BufferFlag::StencilBit)
		openglBufferBit |= GL_STENCIL_BUFFER_BIT;

	return openglBufferBit;
}

uint32_t Rhi::AttachementToOpenglAttachement(const Attachment::Attachment attachment)
{
	switch (attachment)
	{
		case Attachment::Color00:
			return GL_COLOR_ATTACHMENT0;
			 
		case Attachment::Color01:
			return GL_COLOR_ATTACHMENT1;
			 
		case Attachment::Color02:
			return GL_COLOR_ATTACHMENT2;
			 
		case Attachment::Color03:
			return GL_COLOR_ATTACHMENT3;
			 
		case Attachment::Color04:
			return GL_COLOR_ATTACHMENT4;
			 
		case Attachment::Color05:
			return GL_COLOR_ATTACHMENT5;
			 
		case Attachment::Color06:
			return GL_COLOR_ATTACHMENT6;
			 
		case Attachment::Color07:
			return GL_COLOR_ATTACHMENT7;
			 
		case Attachment::Color08:
			return GL_COLOR_ATTACHMENT8;
			 
		case Attachment::Color09:
			return GL_COLOR_ATTACHMENT9;
			 
		case Attachment::Color10:
			return GL_COLOR_ATTACHMENT10;
			 
		case Attachment::Color11:
			return GL_COLOR_ATTACHMENT11;
			 
		case Attachment::Color12:
			return GL_COLOR_ATTACHMENT12;
			 
		case Attachment::Color13:
			return GL_COLOR_ATTACHMENT13;
			 
		case Attachment::Color14:
			return GL_COLOR_ATTACHMENT14;
			 
		case Attachment::Color15:
			return GL_COLOR_ATTACHMENT15;
			 
		case Attachment::Color16:
			return GL_COLOR_ATTACHMENT16;
			 
		case Attachment::Color17:
			return GL_COLOR_ATTACHMENT17;
			 
		case Attachment::Color18:
			return GL_COLOR_ATTACHMENT18;
			 
		case Attachment::Color19:
			return GL_COLOR_ATTACHMENT19;
			 
		case Attachment::Color20:
			return GL_COLOR_ATTACHMENT20;
			 
		case Attachment::Depth:
			return GL_DEPTH_ATTACHMENT;
			  
		case Attachment::Stencil:
			return GL_STENCIL_ATTACHMENT;
			 
		case Attachment::DepthAndStencil:
			return GL_DEPTH_STENCIL_ATTACHMENT;
	}

	return GL_COLOR_ATTACHMENT0;
}

uint32_t Rhi::CubeMapFacesToOpengl(const CubeMapFace cubeMapFace)
{
	switch (cubeMapFace)
	{
		case CubeMapFace::CubeMapPositiveX:
			return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		
		case CubeMapFace::CubeMapNegativeX:
			return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		
		case CubeMapFace::CubeMapPositiveY:
			return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		
		case CubeMapFace::CubeMapNegativeY:
			return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		
		case CubeMapFace::CubeMapPositiveZ:
			return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		
		case CubeMapFace::CubeMapNegativeZ:
			return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

		case CubeMapFace::Size:
			break;
	}

	return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
}

uint32_t Rhi::MemoryBarrierToOpengl(const GpuMemoryBarrier memoryBarrier)
{
	uint32_t memoryBarrierOpengl = 0;
		
	if (memoryBarrier & AllBarrierBits)
	{
		return GL_ALL_BARRIER_BITS; 
	}

	if (memoryBarrier & VertexAttribArrayBarrierBit)
		memoryBarrierOpengl |= GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT;
		
	if (memoryBarrier & ElementArrayBarrierBit)
		memoryBarrierOpengl |= GL_ELEMENT_ARRAY_BARRIER_BIT;
				
	if (memoryBarrier & UniformBarrierBit)
		memoryBarrierOpengl |= GL_UNIFORM_BARRIER_BIT;
	
	if (memoryBarrier & TextureFetchBarrierBit)
		memoryBarrierOpengl |= GL_TEXTURE_FETCH_BARRIER_BIT;
	
	if (memoryBarrier & ShaderImageAccessBarrierBit)
		memoryBarrierOpengl |= GL_SHADER_IMAGE_ACCESS_BARRIER_BIT;
	
	if (memoryBarrier & CommandBarrierBit)
		memoryBarrierOpengl |= GL_COMMAND_BARRIER_BIT;

	if (memoryBarrier & PixelBufferBarrierBit)
		memoryBarrierOpengl |= GL_PIXEL_BUFFER_BARRIER_BIT;
	
	if (memoryBarrier & TextureUpdateBarrierBit)
		memoryBarrierOpengl |= GL_TEXTURE_UPDATE_BARRIER_BIT;

	if (memoryBarrier & BufferUpdateBarrierBit)
		memoryBarrierOpengl |= GL_BUFFER_UPDATE_BARRIER_BIT;

	if (memoryBarrier & ClientMappedBufferBarrierBit)
		memoryBarrierOpengl |= GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT;

	if (memoryBarrier & FramebufferBarrierBit)
		memoryBarrierOpengl |= GL_FRAMEBUFFER_BARRIER_BIT;

	if (memoryBarrier & TransformFeedbackBarrierBit)
		memoryBarrierOpengl |= GL_TRANSFORM_FEEDBACK_BARRIER_BIT;
	
	if (memoryBarrier & AtomicCounterBarrierBit)
		memoryBarrierOpengl |= GL_ATOMIC_COUNTER_BARRIER_BIT;

	if (memoryBarrier & ShaderStorageBarrierBit)
		memoryBarrierOpengl |= GL_SHADER_STORAGE_BARRIER_BIT;
	
	if (memoryBarrier & QueryBufferBarrierBit)
		memoryBarrierOpengl |= GL_QUERY_BUFFER_BARRIER_BIT;
	
	return memoryBarrierOpengl;
}

uint32_t Rhi::GetImageAccessOpengl(const ImageAccess imageAcess)
{
	switch (imageAcess)
	{
		case ImageAccess::ReadOnly: 
			return GL_READ_ONLY;
		
		case ImageAccess::WriteOnly: 
			return GL_WRITE_ONLY;
		
		case ImageAccess::ReadWrite: 
			return GL_READ_WRITE;
	}

	return GL_READ_WRITE;
}

uint32_t Rhi::FrontFaceToOpenglFrontFace(const FrontFace::FrontFace frontFace)
{
	switch (frontFace)
	{
		case FrontFace::CW:
			return GL_CW;

		case FrontFace::CCW:
			return GL_CCW;
	}

	return GL_CW;
}

uint32_t Rhi::CullFaceToOpenglCullFace(const CullFace::CullFace cullFace)
{
	switch (cullFace)
	{
		case CullFace::None:
			return GL_NONE;
		
		case CullFace::Front:
			return GL_FRONT;
		
		case CullFace::Back:
			return GL_BACK;
		
		case CullFace::FrontAndBack:
			return GL_FRONT_AND_BACK;
	}

	return GL_FRONT_AND_BACK;
}

void Rhi::ComputeDepthFunction(DepthFunction::DepthFunction depthFunction)
{
	if (depthFunction != DepthFunction::Disable)
	{
		DepthTest(true);
		glDepthFunc(GetOpengDepthEnum(depthFunction));
	}	
	else
	{
		DepthTest(false);
	}
}

uint32_t Rhi::GetOpengDepthEnum(const DepthFunction::DepthFunction depthFunction)
{
	switch (depthFunction)
	{
		case DepthFunction::Always:
			return GL_ALWAYS;

		case DepthFunction::Never:
			return GL_NEVER;

		case DepthFunction::Less:
			return GL_LESS;

		case DepthFunction::Equal:
			return GL_EQUAL;

		case DepthFunction::LessEqual:
			return GL_LEQUAL;

		case DepthFunction::Greater:
			return GL_GREATER;

		case DepthFunction::NotEqual:
			return GL_NOTEQUAL;

		case DepthFunction::GreaterEqual:
			return GL_GEQUAL;

		case DepthFunction::Disable:
			return GL_LESS;
	}

	return GL_LESS;
}

uint32_t Rhi::BlendEquationToOpengl(const BlendEquation::BlendEquation blendEquation)
{
	switch (blendEquation)
	{
		case BlendEquation::Add:
			return GL_FUNC_ADD;
			
		case BlendEquation::Sub:
			return GL_FUNC_SUBTRACT;
			
		case BlendEquation::ReverSub:
			return GL_FUNC_REVERSE_SUBTRACT;
			
		case BlendEquation::Min:
			return GL_MIN;
			
		case BlendEquation::Max:
			return GL_MAX;
	}

	return GL_FUNC_ADD;
}

uint32_t Rhi::GetOpenglTextureWrapper(const TextureWrapping::TextureWrapping textureWrapping)
{
	switch (textureWrapping)
	{
		case TextureWrapping::None:
			return GL_NONE;
		
		case TextureWrapping::Repeat:
			return GL_REPEAT;
			
		case TextureWrapping::MirroredRepeat:
			return GL_MIRRORED_REPEAT;
		
		case TextureWrapping::ClampToEdge:
			return GL_CLAMP_TO_EDGE;
		
		case TextureWrapping::ClampToBorder:
			return GL_CLAMP_TO_BORDER;
	}

	return GL_REPEAT;
}

void Rhi::ComputeTextureFiltering(const TextureType::TextureType textureType, const uint32_t textureId, const TextureCreateInfo& textureCreateInfo)
{
	const GLint openglTextureFilter = static_cast<GLint>(GetOpenglTextureFilter(textureCreateInfo.filtering));
	
	switch (textureType)
	{
		case TextureType::Texture1D:
			Logger::LogError("Unimplemented behaviours");
			break;

		case TextureType::Texture2DArray:
		case TextureType::Texture2D:
			if(textureCreateInfo.filtering == TextureFiltering::LinearMimMapLinear)
			{
				glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}
			else
			{
				glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, openglTextureFilter);
			}
			glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, openglTextureFilter);
			break;
		 
		case TextureType::Texture3D:
			Logger::LogError("Unimplemented behaviours");
			break;
		 
		case TextureType::Texture1DArray:
			Logger::LogError("Unimplemented behaviours");
			break;
		
		case TextureType::TextureRectangle:
			Logger::LogError("Unimplemented behaviours");
			break;

		case TextureType::TextureCubeMapArray:
		case TextureType::TextureCubeMap:
			if (openglTextureFilter == TextureFiltering::LinearMimMapLinear)
			{
				glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else
			{
				glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, openglTextureFilter);
				glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, openglTextureFilter);
			}
			break;
	
		case TextureType::TextureBuffer:
			Logger::LogError("Unimplemented behaviours");
			break;
		 
		case TextureType::Texture2DMultisample:
			Logger::LogError("Unimplemented behaviours");
			break;
		 
		case TextureType::Texture2DMultisampleArray:
			Logger::LogError("Unimplemented behaviours");
			break;
	}
}

void Rhi::ComputeTextureWrapping(const TextureType::TextureType textureType, const uint32_t textureId , const TextureCreateInfo& textureCreateInfo)
{
	const GLint openglTextureWrapper = static_cast<GLint>(GetOpenglTextureWrapper(textureCreateInfo.wrapping));

	switch (textureType)
	{
		case TextureType::Texture1D:
			Logger::LogError("Unimplemented behaviours");
			break;

		case TextureType::Texture2DArray:
		case TextureType::Texture2D:
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, openglTextureWrapper);
		    glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, openglTextureWrapper);
			if (textureCreateInfo.wrapping == TextureWrapping::ClampToBorder)
			{
				glTextureParameterfv(textureId, GL_TEXTURE_BORDER_COLOR, textureCreateInfo.borderColor);  
			}
			break;
		
		case TextureType::Texture3D:
			Logger::LogError("Unimplemented behaviours");
			break;
		
		case TextureType::Texture1DArray:
			Logger::LogError("Unimplemented behaviours");
			break;
		
		case TextureType::TextureRectangle:
			Logger::LogError("Unimplemented behaviours");
			break;

		case TextureType::TextureCubeMapArray:
		case TextureType::TextureCubeMap:
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, openglTextureWrapper);
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, openglTextureWrapper);
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_R, openglTextureWrapper);
			break;
		
		case TextureType::TextureBuffer:
			Logger::LogError("Unimplemented behaviours");
			break;
		
		case TextureType::Texture2DMultisample:
			Logger::LogError("Unimplemented behaviours");
			break;
		
		case TextureType::Texture2DMultisampleArray:
			Logger::LogError("Unimplemented behaviours");
			break;
	}
}

void Rhi::AllocTexture(const TextureType::TextureType textureType, const uint32_t textureId, const TextureCreateInfo& textureCreateInfo)
{
	const GLenum internalFormat = GetOpenglInternalFormat(textureCreateInfo.internalFormat);
	const GLenum textureFormat = GetOpenGlTextureFormat(textureCreateInfo.format);
	const GLsizei width = textureCreateInfo.size.x;
	const GLsizei height = textureCreateInfo.size.y;
	const GLsizei level = static_cast<int32_t>(textureCreateInfo.mipMaplevel);
	const GLsizei depth = static_cast<int32_t>(textureCreateInfo.depth);

	switch (textureType)
	{
		case TextureType::Texture1D:
			Logger::LogError("Unimplemented behaviours");
			break;
		
		case TextureType::Texture2D:
			
			glTextureStorage2D(textureId,level,internalFormat,width,height);
		
			if (IsDataValid(textureCreateInfo.datas))
			{
				glTextureSubImage2D(textureId, 0, 0, 0, width, height,
				GetOpenGlTextureFormat(textureCreateInfo.format), GetOpenglDataType(textureCreateInfo.dataType),
				textureCreateInfo.datas.at(0));
			}
			break;
		
		case TextureType::Texture3D:
			Logger::LogError("Unimplemented behaviours");
			break;
		
		case TextureType::Texture1DArray:
			Logger::LogError("Unimplemented behaviours");
			break;
		
		case TextureType::Texture2DArray:
			glTextureStorage3D(textureId, level, internalFormat, width, height, depth);
		
			if (IsDataValid(textureCreateInfo.datas))
				glTextureSubImage3D(textureId, 0, 0, 0, 0, width, height, depth, internalFormat, textureFormat, textureCreateInfo.datas.at(0));
			break;

		case TextureType::TextureRectangle:
			Logger::LogError("Unimplemented behaviours");
			break;

		case TextureType::TextureCubeMap:
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
			if (IsDataValid(textureCreateInfo.datas, 6))
			{
				for (size_t i = 0; i < textureCreateInfo.datas.size(); i++)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<GLint>(i), static_cast<GLint>(textureCreateInfo.mipMaplevel), static_cast<GLint>(internalFormat),
						width, height, 0, textureFormat, GetOpenglDataType(textureCreateInfo.dataType), textureCreateInfo.datas.at(i));
				}
			}
			else
			{
				for (size_t i = 0; i < 6; i++)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<GLint>(i), static_cast<GLint>(textureCreateInfo.mipMaplevel), static_cast<GLint>(internalFormat),
						width, height, 0, textureFormat, GetOpenglDataType(textureCreateInfo.dataType),nullptr);
				}
			}
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			break;
		
		case TextureType::TextureCubeMapArray:
			glTextureStorage3D(textureId, level, internalFormat, width, height, depth * 6);
			break;

		case TextureType::TextureBuffer:
			Logger::LogError("Unimplemented behaviours");
			break;

		case TextureType::Texture2DMultisample:
			Logger::LogError("Unimplemented behaviours");
			break;

		case TextureType::Texture2DMultisampleArray:
			Logger::LogError("Unimplemented behaviours");
			break;
	}
}

bool Rhi::IsDataValid(const std::vector<void*>& data, const size_t wantedSize)
{
	if (data.empty())
		return false;
	
	if (data.size() < wantedSize)
		return false;
	
	for (size_t i = 0; i < wantedSize;i++)
	{
		if (data.at(i) == nullptr)
			return false;
	}
	return true;
}


void Rhi::DestroyTexture(const uint32_t textureId)
{
	glDeleteTextures(1, &textureId);
}

void Rhi::BindTexture(const uint32_t unit, const uint32_t textureId)
{
	glBindTextureUnit(unit, textureId);
}

uint32_t Rhi::CreateFrameBuffer()
{
	uint32_t frameBufferId;
	glCreateFramebuffers(1, &frameBufferId);
	return frameBufferId;
}

void Rhi::AttachsTextureToFrameBuffer(const RenderPass& renderPass, const Framebuffer& frameBuffer, const std::vector<const Texture*>& attachments)
{
	const uint32_t frameBufferId = frameBuffer.GetId();
	const std::vector<RenderTargetInfo>& renderTargetInfos = renderPass.renderPassAttachments;
	std::vector<GLenum> openglAttachmentsdraw;
	
	for (uint32_t i = 0; i < renderTargetInfos.size(); i++)
	{
		GLenum openglAttachment = AttachementToOpenglAttachement(renderTargetInfos[i].attachment);

		switch (renderTargetInfos[i].attachment)
		{
			case Attachment::Color00:
			case Attachment::Color01:
			case Attachment::Color02:
			case Attachment::Color03:
			case Attachment::Color04:
			case Attachment::Color05:
			case Attachment::Color06:
			case Attachment::Color07:
			case Attachment::Color08:
			case Attachment::Color09:
			case Attachment::Color10:
			case Attachment::Color11:
			case Attachment::Color12:
			case Attachment::Color13:
			case Attachment::Color14:
			case Attachment::Color15:
			case Attachment::Color16:
			case Attachment::Color17:
			case Attachment::Color18:
			case Attachment::Color19:
			case Attachment::Color20:
				if (renderTargetInfos[i].isDrawingOn)
					openglAttachmentsdraw.push_back(openglAttachment);
				break;

			case Attachment::Depth:
			case Attachment::Stencil:
			case Attachment::DepthAndStencil:
				break;
		}
		glNamedFramebufferTexture(frameBufferId,openglAttachment, attachments.at(i)->GetId(), 0);
	}

	glNamedFramebufferDrawBuffers(frameBufferId, static_cast<int32_t>(openglAttachmentsdraw.size()), openglAttachmentsdraw.data());

}

void Rhi::DestroyFrameBuffer(const uint32_t frameBufferId)
{
	if (glIsFramebuffer(frameBufferId))
		glDeleteFramebuffers(1, &frameBufferId);
}

void Rhi::BlitFrameBuffer(const uint32_t readBuffer, const uint32_t targetBuffer, const Vector2i srcTopLeft, const Vector2i srcBottomRight, const Vector2i targetTopLeft, const Vector2i targetBottomRight,
	const BufferFlag::BufferFlag bufferFlag, const TextureFiltering::TextureFiltering textureFiltering)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, readBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetBuffer);

	glBlitFramebuffer(
		srcTopLeft.x, srcTopLeft.y,
		srcBottomRight.x, srcBottomRight.y,
		targetTopLeft.x, targetTopLeft.y,
		targetBottomRight.x, targetBottomRight.y, GetOpenglBufferBit(bufferFlag), GetOpenglTextureFilter(textureFiltering)
	);
}

void Rhi::BindFrameBuffer(const uint32_t frameBufferId)
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
}

void Rhi::UnbindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Rhi::AttachTextureToFrameBufferLayer(const uint32_t bufferId, const Attachment::Attachment attachment, const uint32_t textureId, const uint32_t level, const uint32_t layer)
{
	const GLenum attachementOpengl = AttachementToOpenglAttachement(attachment);
	glNamedFramebufferTextureLayer(bufferId, attachementOpengl, textureId, static_cast<GLsizei>(level), static_cast<GLint>(layer));
}

void Rhi::AttachTextureToFrameBuffer(const uint32_t bufferId, const Attachment::Attachment attachment, const uint32_t textureId, const uint32_t level)
{
	const GLenum attachementOpengl = AttachementToOpenglAttachement(attachment);
	glNamedFramebufferTexture(bufferId, attachementOpengl, textureId, static_cast<GLsizei>(level));
	
	if (attachment != Attachment::Depth && attachment != Attachment::DepthAndStencil && attachment != Attachment::Stencil)
	{
		glNamedFramebufferDrawBuffers(bufferId, 1, &attachementOpengl);
	}
}

void Rhi::AttachTextureToFrameBuffer(const uint32_t bufferId, const Attachment::Attachment attachment, const CubeMapFace cubeMapFace, const uint32_t textureId, const uint32_t level
)
{
	const GLenum attachement = AttachementToOpenglAttachement(attachment);
	glNamedFramebufferTextureLayer(bufferId,attachement, textureId, static_cast<GLsizei>(level), static_cast<GLint>(cubeMapFace));
	
	if (attachment != Attachment::Depth && attachment != Attachment::DepthAndStencil && attachment != Attachment::Stencil)
	{
		glNamedFramebufferDrawBuffers(bufferId, 1, &attachement);
	}
}

void Rhi::SetFrameBufferDraw(const uint32_t frameBufferid, const uint32_t value)
{
	glNamedFramebufferReadBuffer(frameBufferid, value);
	glNamedFramebufferDrawBuffer(frameBufferid, value);
}

void Rhi::GetPixelFromAttachement(const uint32_t attachmentIndex, const Vector2i position, const TextureFormat::TextureFormat textureFormat, const DataType::DataType dataType, void* const output)
{
	const GLenum format = GetOpenGlTextureFormat(textureFormat);
	const GLenum dataTypeOpengl = GetOpenglDataType(dataType);

	glReadBuffer(GL_COLOR_ATTACHMENT0 + static_cast<GLint>(attachmentIndex)); 
	glReadPixels(position.x, position.y, 1, 1, format, dataTypeOpengl, output);
}

void Rhi::SwapBuffers()
{
	glfwSwapBuffers(Window::GetHandle());
}

uint32_t Rhi::GetOpenglShaderType(const ShaderType::ShaderType shaderType)
{
	switch (shaderType)
	{
		case ShaderType::Vertex:
			return GL_VERTEX_SHADER;
			
		case ShaderType::Fragment:
			return GL_FRAGMENT_SHADER;
			
		case ShaderType::Geometry:
			return GL_GEOMETRY_SHADER;
			
		case ShaderType::Compute:
			return GL_COMPUTE_SHADER;

		default:
			throw std::invalid_argument("Invalid shader type");
	}
}

std::string Rhi::GetShaderTypeToString(const ShaderType::ShaderType shaderType)
{
	switch (shaderType)
	{
		case ShaderType::Vertex:
			return "VERTEX_SHADER";

		case ShaderType::Fragment:
			return "FRAGMENT_SHADER";

		case ShaderType::Geometry:
			return "GEOMETRY_SHADER";

		case ShaderType::Compute:
			return "COMPUTE_SHADER";
	}
	
	throw std::invalid_argument("Invalid shader type");
}

uint32_t Rhi::GetOpenglTextureType(const TextureType::TextureType textureType)
{
	switch (textureType)
	{
		case TextureType::Texture1D:
			return GL_TEXTURE_1D;

		case TextureType::Texture2D:
			return GL_TEXTURE_2D;

		case TextureType::Texture3D:
			return GL_TEXTURE_3D;

		case TextureType::Texture1DArray:
			return GL_TEXTURE_1D_ARRAY;

		case TextureType::Texture2DArray:
			return GL_TEXTURE_2D_ARRAY;

		case TextureType::TextureRectangle:
			return GL_TEXTURE_RECTANGLE;

		case TextureType::TextureCubeMap:
			return GL_TEXTURE_CUBE_MAP;

		case TextureType::TextureCubeMapArray:
			return GL_TEXTURE_CUBE_MAP_ARRAY;

		case TextureType::TextureBuffer:
			return GL_TEXTURE_BUFFER;

		case TextureType::Texture2DMultisample:
			return GL_TEXTURE_2D_MULTISAMPLE;

		case TextureType::Texture2DMultisampleArray:
			return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
	}

	return 0;
}

uint32_t Rhi::GetOpenglInternalFormat(const TextureInternalFormat::TextureInternalFormat textureFormat)
{
	switch (textureFormat)
	{
		case TextureInternalFormat::R8:
			return GL_R8;

		case TextureInternalFormat::R16:
			return GL_R16;

		case TextureInternalFormat::Rg8:
			return GL_RG8;

		case TextureInternalFormat::Rg16:
			return GL_RG16;

		case TextureInternalFormat::Rgb8:
			return GL_RGB8;

		case TextureInternalFormat::Rgb16:
			return GL_RGB16;

		case TextureInternalFormat::Rgba8:
			return GL_RGBA8;

		case TextureInternalFormat::Rgba16:
			return GL_RGBA16;

		case TextureInternalFormat::R16F:
			return GL_R16F;

		case TextureInternalFormat::Rg16F:
			return GL_RG16F;

		case TextureInternalFormat::Rgb16F:
			return GL_RGB16F;

		case TextureInternalFormat::Rgba16F:
			return GL_RGBA16F;

		case TextureInternalFormat::R32F:
			return GL_R32F;
		
		case TextureInternalFormat::R32Uint:
			return GL_R32UI;
		
		case TextureInternalFormat::Srgb:
			return GL_SRGB8;
		
		case TextureInternalFormat::R11FG11FB10F:
			return GL_R11F_G11F_B10F;
		case TextureInternalFormat::Rgba32F:
			return GL_RGBA32F;
		
		case TextureInternalFormat::DepthComponent16:
			return GL_DEPTH_COMPONENT16;
		
		case TextureInternalFormat::DepthComponent24:
			return GL_DEPTH_COMPONENT24;
		
		case TextureInternalFormat::DepthComponent32:
			return GL_DEPTH_COMPONENT32;
		
		case TextureInternalFormat::DepthComponent32F:
			return GL_DEPTH_COMPONENT32F;
		
		case TextureInternalFormat::Depth24Stencil8:
			return GL_DEPTH24_STENCIL8;

		case TextureInternalFormat::DepthComponent32FStencil8:
			return GL_DEPTH32F_STENCIL8;
	}

	Logger::LogError("Texture InternalFormat not supported, defaulting to RGB");
	return GL_RGB;
}

uint32_t Rhi::GetOpenGlTextureFormat(const TextureFormat::TextureFormat textureFormat)
{
	switch (textureFormat)
	{
		case TextureFormat::Red: 
			return GL_RED;
		
		case TextureFormat::RedGreen:
			return GL_RG;
		
		case TextureFormat::Rgb:
			return GL_RGB;
	
		case TextureFormat::Rgba:
			return GL_RGBA;

		case TextureFormat::DepthComponent:
			return GL_DEPTH_COMPONENT;
	}
	
	return GL_RGB;
}

void Rhi::LogComputeShaderInfo()
{
	int32_t workGroupCount[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &workGroupCount[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &workGroupCount[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &workGroupCount[2]);
	Logger::LogDebug("Max work groups per compute shader x: {} y: {} x: {}", workGroupCount[0], workGroupCount[1], workGroupCount[2]);
	
	int32_t workGroupSize[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &workGroupSize[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &workGroupSize[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &workGroupSize[2]);
	Logger::LogDebug("Max work group sizes  x: {} y: {} x: {}", workGroupSize[0], workGroupSize[1], workGroupSize[2]); 

	int32_t workGroupInvocation;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &workGroupInvocation);
	Logger::LogDebug("Max invocations count per work group: {} ", workGroupInvocation);
}

void Rhi::IsShaderValid(const uint32_t shaderId)
{
	if (!m_ShaderMap.contains(shaderId) || !glIsProgram(shaderId))
	{
		Logger::LogFatal("No shader with id #{}", shaderId);
		throw std::runtime_error("No shader with this id");
	}
}

int32_t Rhi::GetUniformInMap(const uint32_t shaderId, const char_t* const uniformKey)
{
	std::map<std::string, uint32_t>& shaderUniformMap = m_ShaderMap.at(shaderId).uniformMap;

	if (shaderUniformMap.contains(uniformKey))
	{
		return static_cast<int32_t>(shaderUniformMap[uniformKey]);
	}

	const GLint location = glGetUniformLocation(shaderId, uniformKey);
	if (location == NullUniformLocation)
	{
		Logger::LogWarning("No uniform with key [{}] in shader #{}", uniformKey, shaderId);
	}
		
	shaderUniformMap[uniformKey] = location;

	return location;
}

uint32_t Rhi::GetOpenglDataType(const DataType::DataType dataType)
{
	switch (dataType)
	{
		
		case DataType::Byte:
			return GL_BYTE;
		
		case DataType::UnsignedByte:
			return GL_UNSIGNED_BYTE;
		
		case DataType::Short:
			return GL_SHORT;
		
		case DataType::UnsignedShort:
			return GL_UNSIGNED_SHORT;
			
		case DataType::Int:
			return GL_INT;
			
		case DataType::UnsignedInt:
			return GL_UNSIGNED_INT ;

		case DataType::Float:
			return GL_FLOAT;
		
		case DataType::Double:
			return GL_DOUBLE;
			
	}

	return GL_UNSIGNED_BYTE;
}

void Rhi::OpenglDebugCallBack(const uint32_t source, const uint32_t type, const uint32_t id, const uint32_t severity, const int32_t, const char_t* const message, const void* const)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return; 

	Logger::LogError("---------------");
	Logger::LogError("Debug message ({}): {}", id,message);
	
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
        	Logger::LogError("Source: API");
    		break;
    	
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        	Logger::LogError("Source: Window System");
    		break;
    	
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
        	Logger::LogError("Source: Shader Compiler");
    		break;
    	
        case GL_DEBUG_SOURCE_THIRD_PARTY:
        	Logger::LogError("Source: Third Party");
    		break;
    	
        case GL_DEBUG_SOURCE_APPLICATION:
        	Logger::LogError("Source: Application");
    		break;
    	
        case GL_DEBUG_SOURCE_OTHER:
        	Logger::LogError("Source: Other");
    		break;

    	default:
    		Logger::LogError("Source: Unknown [{}]", source);
    }

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
        	Logger::LogError("Type: Error");
    		break;
    	
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			Logger::LogError("Type: Deprecated Behaviour");
    		break;
    	
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			Logger::LogError("Type: Undefined Behaviour"); 
    		break;
    	
        case GL_DEBUG_TYPE_PORTABILITY:
			Logger::LogError("Type: Portability"); 
    		break;
    	
        case GL_DEBUG_TYPE_PERFORMANCE:
			Logger::LogError("Type: Performance"); 
    		break;
    	
        case GL_DEBUG_TYPE_MARKER:
			Logger::LogError("Type: Marker"); 
    		break;
    	
        case GL_DEBUG_TYPE_PUSH_GROUP:
			Logger::LogError("Type: Push Group"); 
    		break;
    	
        case GL_DEBUG_TYPE_POP_GROUP:
			Logger::LogError("Type: Pop Group"); 
    		break;
    	
        case GL_DEBUG_TYPE_OTHER:
			Logger::LogError("Type: Other"); 
    		break;

    	default:
			Logger::LogError("Type: Unknown [{}]", type); 
    }
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
        	Logger::LogError("Severity: high");
    		break;

        case GL_DEBUG_SEVERITY_MEDIUM:
        	Logger::LogError("Severity: medium");
    		break;

        case GL_DEBUG_SEVERITY_LOW:
        	Logger::LogError("Severity: low");
    		break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
        	Logger::LogError("Severity: notification");
    		break;

    	default:
        	Logger::LogError("Severity: Unknown [{}]", severity);
    }
}


void Rhi::Initialize()
{
	gladLoadGL();
	DepthTest(m_Depth);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);  

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
	glDebugMessageCallback(reinterpret_cast<GLDEBUGPROC>(OpenglDebugCallBack), nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	LogComputeShaderInfo();
#endif
}

void Rhi::Shutdown()
{
	// ReSharper disable once CppDiscardedPostfixOperatorResult
	for (decltype(m_ModelMap)::iterator it = m_ModelMap.begin(); it != m_ModelMap.end(); it++)
	{
		DestroyModel(it->first);
	}

	delete m_CameraUniform;
	delete m_ModelUniform;
	delete m_LightUniform;
	delete m_MaterialUniform;
	delete m_AnimationBuffer;
}

void Rhi::PrepareRendering()
{
	m_CameraUniform = new UniformBuffer;
	m_CameraUniform->Allocate(sizeof(CameraUniformData), nullptr);
	m_CameraUniform->Bind(0);
	
	m_ModelUniform = new UniformBuffer;
	m_ModelUniform->Allocate(sizeof(ModelUniformData), nullptr);
	m_ModelUniform->Bind(1);
	
	m_LightUniform = new UniformBuffer;
	m_LightUniform->Allocate(sizeof(GpuLightData), nullptr);
	m_LightUniform->Bind(2);
	
	m_MaterialUniform = new UniformBuffer;
	m_MaterialUniform->Allocate(sizeof(MaterialData),nullptr);
	m_MaterialUniform->Bind(4);

	m_AnimationBuffer = new UniformBuffer();
	m_AnimationBuffer->Allocate(sizeof(SkinnedMeshGPUData),nullptr);
	m_AnimationBuffer->Bind(5);

	skyBoxParser.Init();
}

void Rhi::SetClearColor(const Vector4& color)
{
	glClearColor(color.x, color.y, color.z, color.w);
}

void Rhi::ClearBuffer(const BufferFlag::BufferFlag bufferFlag)
{
	glClear(GetOpenglBufferBit(bufferFlag));
}

void Rhi::UpdateModelUniform(const ModelUniformData& modelUniformData)
{	
	m_ModelUniform->Update(sizeof(ModelUniformData), 0, modelUniformData.model.Raw());
}

void Rhi::UpdateCameraUniform(const CameraUniformData& cameraUniformData)
{
	m_CameraUniform->Update(sizeof(CameraUniformData), 0, cameraUniformData.view.Raw());
}

void Rhi::UpdateAninationUniform(const SkinnedMeshGPUData& skinnedMeshGPUData)
{
	m_LightUniform->Update(sizeof(SkinnedMeshGPUData), 0, skinnedMeshGPUData.BoneMatrices->Raw());
}

void Rhi::UpdateLight(const GpuLightData& lightData)
{
	m_LightUniform->Update(sizeof(GpuLightData), 0, &lightData.nbrOfPointLight);
}

void Rhi::BindMaterial(const Material& material)
{
	MaterialData materialData;
	
	materialData.hasAlbedoMap = static_cast<int32_t>(material.albedoTexture.IsValid());
	materialData.hasMetallicMap =  static_cast<int32_t>(material.metallicTexture.IsValid());
	materialData.hasRoughnessMap =  static_cast<int32_t>(material.roughnessTexture.IsValid());
	materialData.hasNormalMap =  static_cast<int32_t>(material.normalTexture.IsValid());
	materialData.hasAmbientOcclusionMap =  static_cast<int32_t>(material.ambientOcclusionTexture.IsValid());
	
	materialData.albedoColor = static_cast<Vector3>(material.albedoColor);
	materialData.emissiveColor = static_cast<Vector3>(material.emissiveColor);
	
	materialData.metallic = material.metallic;
	materialData.roughness = material.roughness;
	materialData.reflectance = material.reflectance;
	materialData.emissive = material.emissive;
	materialData.ambientOcclusion = material.ambientOcclusion;
	
	constexpr size_t size = sizeof(MaterialData);
	m_MaterialUniform->Update(size, 0, &materialData);
}

TextureFormat::TextureFormat Rhi::GetTextureFormatFromChannels(const uint32_t channels)
{
	switch (channels)
	{
		case 1:
			return TextureFormat::Red;

		case 3:
			return TextureFormat::Rgb;

		case 4:
			return TextureFormat::Rgba;

		default:
			return TextureFormat::Rgb;
	}
}

void Rhi::DepthTest(const bool_t value)
{
	value ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	m_Depth = value;
}


void Rhi::GetCubeMapViewMatrices(std::array<Matrix, 6>* viewsMatricies)
{
	*viewsMatricies =
	{
		Matrix::LookAt(Vector3(), -Vector3::UnitX(), -Vector3::UnitY()), // CubeMapPositiveX
		Matrix::LookAt(Vector3(),  Vector3::UnitX(), -Vector3::UnitY()), // CubeMapNegativeX
		Matrix::LookAt(Vector3(), -Vector3::UnitY(), -Vector3::UnitZ()), // CubeMapPositiveY
		Matrix::LookAt(Vector3(),  Vector3::UnitY(),  Vector3::UnitZ()), // CubeMapNegativeY
		Matrix::LookAt(Vector3(),  Vector3::UnitZ(), -Vector3::UnitY()), // CubeMapPositiveZ
		Matrix::LookAt(Vector3(), -Vector3::UnitZ(), -Vector3::UnitY()), // CubeMapNegativeZ
	};
}

void Rhi::DispatchCompute(const uint32_t numberOfGroupX, const uint32_t numberOfGroupY, const uint32_t numberOfGroupZ)
{
	glDispatchCompute(numberOfGroupX, numberOfGroupY, numberOfGroupZ);
}

void Rhi::SetGpuMemoryBarrier(const GpuMemoryBarrier memoryBarrier)
{
	glMemoryBarrier(MemoryBarrierToOpengl(memoryBarrier));
}

void Rhi::BindImageTexture(const uint32_t unit, const uint32_t texture,	const uint32_t level, const bool_t layered,	const uint32_t layer, const ImageAccess imageAcess,	const TextureInternalFormat::TextureInternalFormat textureInternalFormat)
{
	const GLenum access = GetImageAccessOpengl(imageAcess);
	const GLenum textureFormatInternal = GetOpenglInternalFormat(textureInternalFormat);

	glBindImageTexture(unit, texture, static_cast<GLint>(level), static_cast<GLboolean>(layered), static_cast<GLint>(layer), access, textureFormatInternal);
}

uint32_t Rhi::CreateTexture(const TextureCreateInfo& textureCreateInfo)
{
	const uint32_t textureId = CreateTextureId(textureCreateInfo.textureType);
	AllocTexture(textureCreateInfo.textureType, textureId, textureCreateInfo);
	ComputeTextureFiltering(textureCreateInfo.textureType, textureId, textureCreateInfo);
	ComputeTextureWrapping(textureCreateInfo.textureType, textureId, textureCreateInfo);
	glGenerateTextureMipmap(textureId);
	return textureId;
}
