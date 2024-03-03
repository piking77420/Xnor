#include "rendering/rhi.hpp"

#include <glad/glad.h>

#include "window.hpp"
#include "GLFW/glfw3.h"
#include "rendering/render_pass.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void Rhi::SetPolygonMode(const PolygonFace face, const PolygonMode mode)
{
	glPolygonMode(static_cast<GLenum>(face), GL_POINT + static_cast<GLenum>(mode));
}

void Rhi::SetViewport(const Vector2i screenSize)
{
	glViewport(0, 0, screenSize.x, screenSize.y);
}

void Rhi::DrawQuad(const uint32_t quadId)
{
	const ModelInternal model = m_ModelMap.at(quadId);
	glBindVertexArray(model.vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

uint32_t Rhi::CreateModel(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indicies)
{
	ModelInternal modelInternal;
	modelInternal.nbrOfVertex = static_cast<uint32_t>(vertices.size());
	modelInternal.nbrOfIndicies = static_cast<uint32_t>(indicies.size()); 
	
	glCreateVertexArrays(1, &modelInternal.vao);

	glCreateBuffers(1, &modelInternal.vbo);
	glCreateBuffers(1, &modelInternal.ebo);

	GLintptr offset = static_cast<GLintptr>(vertices.size() * sizeof(Vertex));
	glNamedBufferData(modelInternal.vbo, offset, vertices.data(), GL_STATIC_DRAW);
	offset = static_cast<GLintptr>(indicies.size() * sizeof(uint32_t));
	glNamedBufferData(modelInternal.ebo, offset, indicies.data(), GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(modelInternal.vao, 0);
	glVertexArrayAttribBinding(modelInternal.vao, 0, 0);
	glVertexArrayAttribFormat(modelInternal.vao, 0, 3, GL_FLOAT, GL_FALSE, 0);

	glEnableVertexArrayAttrib(modelInternal.vao, 1);
	glVertexArrayAttribBinding(modelInternal.vao, 1, 0);
	glVertexArrayAttribFormat(modelInternal.vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));

	glEnableVertexArrayAttrib(modelInternal.vao, 2);
	glVertexArrayAttribBinding(modelInternal.vao, 2, 0);
	glVertexArrayAttribFormat(modelInternal.vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, textureCoord));

	glEnableVertexArrayAttrib(modelInternal.vao, 3);
	glVertexArrayAttribBinding(modelInternal.vao, 3, 0);
	glVertexArrayAttribFormat(modelInternal.vao, 3, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, tangent));

	glEnableVertexArrayAttrib(modelInternal.vao, 4);
	glVertexArrayAttribBinding(modelInternal.vao, 4, 0);
	glVertexArrayAttribFormat(modelInternal.vao, 4, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, bitangent));
	
	glVertexArrayVertexBuffer(modelInternal.vao, 0, modelInternal.vbo, 0, sizeof(Vertex));
	glVertexArrayElementBuffer(modelInternal.vao, modelInternal.ebo);
	
	const uint32_t modelId = modelInternal.vao;
	
	m_ModelMap.emplace(modelId, modelInternal);
	
	return modelId;
}

bool Rhi::DestroyModel(const uint32_t modelId)
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
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);
			Logger::LogError("Error while compiling shader of type {}: {}", type.c_str(), infoLog);
		}
	}
	else
	{
		glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderId, 1024, nullptr, infoLog);
			Logger::LogError("Error while linking shader program of type {}: {}", type.c_str(), infoLog);
		}
	}
}

uint32_t Rhi::CreateShaders(const std::vector<ShaderCode>& shaderCodes,const ShaderCreateInfo& shaderCreateInfo)
{
	const uint32_t programId = glCreateProgram();
	std::vector<uint32_t> shaderIds(shaderCodes.size());
	

	for (size_t i = 0; i < shaderCodes.size(); i++)
	{
		const ShaderCode& code = shaderCodes[i];
		if (code.code == nullptr)
			continue;

		uint32_t& shaderId = shaderIds[i];
		shaderId = glCreateShader(GetOpenglShaderType(code.type));
		glShaderSource(shaderId, 1, &code.code, &code.codeLength);
		glCompileShader(shaderId);
		CheckCompilationError(shaderId, GetShaderTypeToString(code.type));
		glAttachShader(programId, shaderId);
	}
	
	glLinkProgram(programId);
	CheckCompilationError(programId, "PROGRAM");

	ShaderInternal shaderInternal;
	shaderInternal.depthFunction = shaderCreateInfo.depthFunction;
	
	m_ShaderMap.emplace(programId,shaderInternal);
	
	return programId;
}

void Rhi::UseShader(const uint32_t shaderId)
{
#ifdef _DEBUG
	IsShaderValid(shaderId);
#endif

	const ShaderInternal& shaderInternal = m_ShaderMap.at(shaderId);
	
	glDepthFunc(GetOpengDepthEnum(shaderInternal.depthFunction));
	
	if(shaderInternal.blendFunction.IsBlanding)
	{
		const uint32_t srcValue =  GetBlendValueOpengl(shaderInternal.blendFunction.sValue);
		const uint32_t destValue =  GetBlendValueOpengl(shaderInternal.blendFunction.dValue);
		glBlendFunc(srcValue,destValue);
	}
	else
	{
		const uint32_t srcValue =  GetBlendValueOpengl(BlendValue::ONE);
		const uint32_t destValue =  GetBlendValueOpengl(BlendValue::ZERO);
		glBlendFunc(srcValue,destValue);
	}
	
	
	glUseProgram(shaderId);
}

void Rhi::UnuseShader()
{
	glUseProgram(0);
}

void Rhi::SetUniform(const UniformType uniformType, const void* data, const uint32_t shaderId, const char* uniformKey)
{
	const GLint uniformLocation = GetUniformInMap(shaderId, uniformKey);

	switch (uniformType)
	{
		case UniformType::Int:
			glUniform1i(uniformLocation, *static_cast<const int32_t*>(data));
			break;
			
		case UniformType::Bool:
			glUniform1i(uniformLocation, *static_cast<const bool*>(data));
			break;
			
		case UniformType::Float:
			glUniform1f(uniformLocation, *static_cast<const GLfloat*>(data));
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

void Rhi::CreateTexture(uint32_t* textureId,TextureType textureType)
{
	glCreateTextures(GetOpenglTextureType(textureType), 1, textureId);
}
uint32_t Rhi::GetOpenglTextureFilter(TextureFiltering textureFiltering)
{
	switch (textureFiltering)
	{
		case TextureFiltering::None:
			return GL_NONE;
			
		case TextureFiltering::Linear:
			return GL_LINEAR;
			
		case TextureFiltering::Nearest:
			return GL_NEAREST;
		
	}
	return GL_LINEAR;
}

uint32_t Rhi::GetBlendValueOpengl(BlendValue blendFunction)
{
	switch (blendFunction)
	{
		case BlendValue::ZERO:
			return GL_ZERO;
			
		case BlendValue::ONE:
			return GL_ONE;
			
		case BlendValue::SRC_COLOR:
			return GL_SRC_COLOR;
			
		case BlendValue::ONE_MINUS_SRC_COLOR:
			return GL_ONE_MINUS_SRC_COLOR;
			
		case BlendValue::DST_COLOR:
			return GL_DST_COLOR;
			
		case BlendValue::ONE_MINUS_DST_COLOR:
			return GL_ONE_MINUS_DST_COLOR;
			
		case BlendValue::SRC_ALPHA:
			return GL_SRC_ALPHA;
			
		case BlendValue::ONE_MINUS_SRC_ALPHA:
			return GL_ONE_MINUS_SRC_ALPHA;
			
		case BlendValue::DST_ALPHA:
			return GL_DST_ALPHA;
			
		case BlendValue::ONE_MINUS_DST_ALPHA:
			return GL_ONE_MINUS_DST_ALPHA;
			
		case BlendValue::CONSTANT_COLOR:
			return GL_CONSTANT_COLOR;
			
		case BlendValue::ONE_MINUS_CONSTANT_COLOR:
			return GL_ONE_MINUS_CONSTANT_COLOR;
			
		case BlendValue::CONSTANT_ALPHA:
			return GL_CONSTANT_ALPHA;
			
		case BlendValue::ONE_MINUS_CONSTANT_ALPHA:
			return GL_ONE_MINUS_CONSTANT_ALPHA;

		default:
			return GL_ONE; 
	}
}

uint32_t Rhi::GetOpengDepthEnum(DepthFunction depthFunction)
{
	switch (depthFunction)
	{
		case DepthFunction::ALWAYS:
			return GL_ALWAYS;
		case DepthFunction::NEVER:
			return GL_NEVER;
		case DepthFunction::LESS:
			return GL_LESS;
		case DepthFunction::EQUAL:
			return GL_EQUAL;
		case DepthFunction::LEAQUAL:
			return GL_LEQUAL;
		case DepthFunction::GREATER:
			return GL_GREATER;
		case DepthFunction::NOTEQUAL:
			return GL_NOTEQUAL;
		case DepthFunction::GEQUAL:
			return GL_GEQUAL;
	}
	return GL_LESS;
}


uint32_t Rhi::GetOpenglTextureWrapper(TextureWrapping textureWrapping)
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

void Rhi::AllocTexture2D(const uint32_t* textureId, const TextureCreateInfo& textureCreateInfo)
{
	glTextureStorage2D(*textureId, 1,
		GetOpenglInternalFormat(textureCreateInfo.textureInternalFormat),
		static_cast<GLsizei>(textureCreateInfo.textureSizeWidth),
		static_cast<GLsizei>(textureCreateInfo.textureSizeHeight)
	);
}

void Rhi::CreateTexture2D(uint32_t* const textureId, const TextureCreateInfo& textureCreateInfo)
{
	CreateTexture(textureId,TextureType::Texture2D);

	const GLint openglTextureFilter =  static_cast<GLint>(GetOpenglTextureFilter(textureCreateInfo.textureFiltering));
	const GLint openglTextureWrapper =  static_cast<GLint>(GetOpenglTextureWrapper(textureCreateInfo.textureWrapping));
	
	glTextureParameteri(*textureId, GL_TEXTURE_MIN_FILTER,openglTextureFilter);
	glTextureParameteri(*textureId, GL_TEXTURE_MAG_FILTER, openglTextureFilter);
	glTextureParameteri(*textureId, GL_TEXTURE_WRAP_S, openglTextureWrapper);
	glTextureParameteri(*textureId, GL_TEXTURE_WRAP_T, openglTextureWrapper);

	AllocTexture2D(textureId,textureCreateInfo);

	if (textureCreateInfo.data != nullptr)
	{
		glTextureSubImage2D(*textureId, 0, 0, 0,
		static_cast<GLsizei>(textureCreateInfo.textureSizeWidth),
		static_cast<GLsizei>(textureCreateInfo.textureSizeHeight),
		GetOpenGlTextureFormat(textureCreateInfo.textureFormat), GetOpenglDataType(textureCreateInfo.dataType), textureCreateInfo.data);
	}
	
	
	glGenerateTextureMipmap(*textureId);
}

void Rhi::DestroyTexture(uint32_t* const textureId)
{
	glDeleteTextures(1, textureId);
	*textureId = 0;
}

void Rhi::BindTexture(const uint32_t unit, const uint32_t textureId)
{
	glBindTextureUnit(unit, textureId);
}

void Rhi::CreateCubeMap(uint32_t* textureId, const CreateCubeMapInfo& createCubeMapInfo)
{
	CreateTexture(textureId,TextureType::TextureCubeMap);
	const GLint openglTextureFilter =  static_cast<GLint>(GetOpenglTextureFilter(createCubeMapInfo.textureFiltering));
	const GLint openglTextureWrapper =  static_cast<GLint>(GetOpenglTextureWrapper(createCubeMapInfo.textureWrapping));
	glTextureParameteri(*textureId, GL_TEXTURE_MIN_FILTER, openglTextureFilter);
	glTextureParameteri(*textureId, GL_TEXTURE_MAG_FILTER, openglTextureFilter);
	glTextureParameteri(*textureId, GL_TEXTURE_WRAP_S, openglTextureWrapper);
	glTextureParameteri(*textureId, GL_TEXTURE_WRAP_T, openglTextureWrapper);
	glTextureParameteri(*textureId, GL_TEXTURE_WRAP_R, openglTextureWrapper);

	
	glBindTexture(GL_TEXTURE_CUBE_MAP, *textureId);
	
	if(createCubeMapInfo.datas == nullptr || createCubeMapInfo.datas == nullptr)
	{
		Logger::LogError("CubeMapCreateInfo is Invalid");
	}
	
	const GLsizei widht = static_cast<GLsizei>(createCubeMapInfo.textureSizeWidth);
	const GLsizei height = static_cast<GLsizei>(createCubeMapInfo.textureSizeHeight);
	
	
	for (size_t i = 0; i < createCubeMapInfo.datas->size(); i++)
	{
		glTexImage2D(
	   GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<GLint>(i), 
	   0, static_cast<GLint>(GetOpenglInternalFormat(createCubeMapInfo.textureInternalFormat)), widht, height, 0, GetOpenGlTextureFormat(createCubeMapInfo.textureFormat),
	   GetOpenglDataType(createCubeMapInfo.dataType), createCubeMapInfo.datas->at(i));
	}
	

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}




void Rhi::CreateFrameBuffer(uint32_t* const frameBufferId, const RenderPass& renderPass, const std::vector<const Texture*>& attechements)
{
	glCreateFramebuffers(1, frameBufferId);

	const std::vector<RenderTargetInfo>& renderTargetInfos = renderPass.renderPassAttachments;
	std::vector<GLenum> openglAttachmentsdraw;
	
	for (uint32_t i = 0; i < renderTargetInfos.size(); i++)
	{
		GLenum openglAttachment = 0 ;
		switch (renderTargetInfos[i].attachment)
		{
			case Attachment::Color01:  // NOLINT(bugprone-branch-clone)
				openglAttachment = GL_COLOR_ATTACHMENT0 + i;
				break;
			
			case Attachment::Color02:
			openglAttachment = GL_COLOR_ATTACHMENT0 + i;
				break;
			
			case Attachment::Color03:
			openglAttachment = GL_COLOR_ATTACHMENT0 + i;
				break;
			
			case Attachment::Color04:
				openglAttachment = GL_COLOR_ATTACHMENT0 + i;
				break;
			
			case Attachment::Depth:
				openglAttachment = GL_DEPTH_ATTACHMENT;
				break;
			
			case Attachment::Stencil:
				openglAttachment = GL_STENCIL_ATTACHMENT;
				break;
			
			case Attachment::DepthAndStencil:
				openglAttachment = GL_DEPTH_STENCIL_ATTACHMENT;
				break;
			
		}
		glNamedFramebufferTexture(*frameBufferId, openglAttachment, attechements.at(i)->GetId(), 0);
		
		if (renderTargetInfos[i].draw)
		{
			openglAttachmentsdraw.push_back(openglAttachment);
		}
		
	}
	glNamedFramebufferDrawBuffers(*frameBufferId, static_cast<int32_t>(openglAttachmentsdraw.size()), openglAttachmentsdraw.data());
	
}

void Rhi::DestroyFrameBuffer(uint32_t* const frameBufferId)
{
	if (glIsFramebuffer(*frameBufferId))
	{
		glDeleteFramebuffers(1, frameBufferId);
		*frameBufferId = 0;
	}
}

void Rhi::BlitFrameBuffer(
	const uint32_t readBuffer,
	const uint32_t targetBuffer,
	const Vector2i src0Size,
	const Vector2i src1Size,
	const Vector2i target0Size,
	const Vector2i target1Size,
	[[maybe_unused]]const Attachment attachmentTarget,
	const TextureFiltering textureFiltering
)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, readBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetBuffer);
	// TODO PARESER ATTACHEMNT TO BUFFER BIT 
	glBlitFramebuffer(
		src0Size.x, src0Size.y,
		src1Size.x, src1Size.y,
		target0Size.x, target0Size.y,
		target1Size.x, target1Size.y, GL_DEPTH_BUFFER_BIT, GetOpenglTextureFilter(textureFiltering)
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

void Rhi::SwapBuffers()
{
	glfwSwapBuffers(Window::GetHandle());
}

uint32_t Rhi::GetOpenglShaderType(const ShaderType shaderType)
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
		
		case ShaderType::Count:
			break;
	}
	
	throw std::invalid_argument("Invalid shader type");
}

std::string Rhi::GetShaderTypeToString(const ShaderType shaderType)
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
		
		case ShaderType::Count:
			break;
	}
	
	throw std::invalid_argument("Invalid shader type");
}

uint32_t Rhi::GetOpenglTextureType(const TextureType textureType)
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

uint32_t Rhi::GetOpenglInternalFormat(const TextureInternalFormat textureFormat)
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

		case TextureInternalFormat::DepthComponent16:
			return GL_DEPTH_COMPONENT16;
		
		case TextureInternalFormat::DepthComponent24:
			return GL_DEPTH_COMPONENT24;
		
		case TextureInternalFormat::DepthComponent32:
			return GL_DEPTH_COMPONENT32;
		
		case TextureInternalFormat::DepthComponent32f:
			return GL_DEPTH_COMPONENT32F;
		
		case TextureInternalFormat::DepthStencil:
			return GL_DEPTH24_STENCIL8;
	}

	Logger::LogError("Texture InternalFormat not supported, defaulting to RGB");
	
	return GL_RGB;
}

uint32_t Rhi::GetOpenGlTextureFormat(const TextureFormat textureFormat)
{
	switch (textureFormat)
	{
		case TextureFormat::Red: 
			return GL_RED;
	
		case TextureFormat::Rgb:
			return GL_RGB;
	
		case TextureFormat::Rgba:
			return GL_RGBA;
	}
	
	return GL_RGB;
}

void Rhi::IsShaderValid(const uint32_t shaderId)
{
	if (!m_ShaderMap.contains(shaderId) || !glIsProgram(shaderId))
	{
		Logger::LogFatal("No shader with id #{}", shaderId);
		throw std::runtime_error("No shader with this id");
	}
}

int32_t Rhi::GetUniformInMap(const uint32_t shaderId, const char* const uniformKey)
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

uint32_t Rhi::GetOpenglDataType(const DataType dataType)
{
	switch (dataType)
	{
		case DataType::Float:
			return GL_FLOAT;

		case DataType::UnsignedByte:
			return GL_UNSIGNED_BYTE;
	}

	return GL_UNSIGNED_BYTE;
}



void Rhi::OpenglDebugCallBack([[maybe_unused]] const uint32_t source,
                              [[maybe_unused]] const uint32_t type,
                              [[maybe_unused]] const uint32_t id,
                              [[maybe_unused]] const uint32_t severity,
                              [[maybe_unused]] const size_t length,
                              [[maybe_unused]] const char_t* const message,
                              [[maybe_unused]] const void* const userParam)
{
	
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
    	return; 

	Logger::LogError("---------------");  // NOLINT(clang-diagnostic-misleading-indentation)
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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glDepthFunc(GL_LESS);
	
#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
	glDebugMessageCallback(reinterpret_cast<GLDEBUGPROC>(OpenglDebugCallBack), nullptr);  // NOLINT(clang-diagnostic-cast-function-type-strict)
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif
	
}


void Rhi::Shutdown()
{
	// ReSharper disable once CppDiscardedPostfixOperatorResult
	for (std::unordered_map<uint32_t, ModelInternal>::iterator it = m_ModelMap.begin() ; it != m_ModelMap.end(); it++)
	{
		DestroyModel(it->first);
	}

	delete m_CameraUniform;
	delete m_ModelUniform;
	delete m_LightUniform;
	delete m_LightShadowMappingUniform;
	delete m_MaterialUniform;
}

void Rhi::PrepareUniform()
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

	m_LightShadowMappingUniform = new UniformBuffer;
	m_LightShadowMappingUniform->Allocate(sizeof(ShadowMappingData), nullptr);
	m_LightUniform->Bind(3);

	m_MaterialUniform = new UniformBuffer;
	m_MaterialUniform->Allocate(sizeof(MaterialData),nullptr);
	m_MaterialUniform->Bind(4);

}

void Rhi::SetClearColor(const Vector4& color)
{
	glClearColor(color.x, color.y, color.z, color.w);
}

void Rhi::ClearColorAndDepth()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Rhi::ClearColor()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Rhi::ClearDepth()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Rhi::UpdateModelUniform(const ModelUniformData& modelUniformData)
{	
	constexpr size_t size = sizeof(ModelUniformData);
	m_ModelUniform->Update(size, 0, modelUniformData.model.Raw());
}

void Rhi::UpdateCameraUniform(const CameraUniformData& cameraUniformData)
{
	m_CameraUniform->Update(sizeof(CameraUniformData), 0, cameraUniformData.view.Raw());
}

void Rhi::UpdateLight(const GpuLightData& lightData)
{
	m_LightUniform->Update(sizeof(GpuLightData), 0, &lightData.nbrOfPointLight);
}

void Rhi::BindMaterial(const Material& material)
{
	MaterialData materialData;
	
	materialData.hasAlbedoMap = static_cast<int32_t>(material.albedo.IsValid());
	materialData.hasNormalmap =  static_cast<int32_t>(material.normalMap.IsValid());
	constexpr size_t size = sizeof(MaterialData);
	m_MaterialUniform->Update(size, 0, &materialData);
}

void Rhi::UpdateShadowMapingData(const ShadowMappingData& shadowMappingData)
{
	m_LightShadowMappingUniform->Update(sizeof(ShadowMappingData), 0, &shadowMappingData);
}

TextureFormat Rhi::GetFormat(const uint32_t textureFormat)
{
	{
		switch (textureFormat)
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
}
