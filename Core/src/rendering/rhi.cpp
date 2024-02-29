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

void Rhi::BindMaterial([[maybe_unused]] const Material& material)
{
}

/*
void RHI::BindMaterial(const Material& material)
{
	material.shader->Use();
	
	
	
	material.shader->Unuse();
}*/

void Rhi::DestroyShader(const uint32_t shaderId)
{
	IsShaderValid(shaderId);
	
	glDeleteShader(shaderId);
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

uint32_t Rhi::CreateShaders(const std::vector<ShaderCode>& shaderCodes)
{
	uint32_t programId = glCreateProgram();
	
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
	m_ShaderMap.emplace(programId, ShaderInternal());

	return programId;
}

void Rhi::UseShader(const uint32_t shaderId)
{
#ifdef _DEBUG
	IsShaderValid(shaderId);
#endif
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

void Rhi::CreateTexture(uint32_t* const textureId, const TextureCreateInfo& textureCreateInfo)
{
	glCreateTextures(GL_TEXTURE_2D, 1, textureId);

	ComputeTextureWrapper(*textureId, textureCreateInfo.textureWrapping);
	ComputeOpenglTextureFilter(*textureId, textureCreateInfo.textureFiltering);

	glTextureStorage2D(*textureId, 1,
		GetOpenglInternalFormat(textureCreateInfo.textureInternalFormat),
		static_cast<GLsizei>(textureCreateInfo.textureSizeWidth),
		static_cast<GLsizei>(textureCreateInfo.textureSizeHeight)
	);
	
	if (!textureCreateInfo.data)
		return;

	glTextureSubImage2D(*textureId, 0, 0, 0,
		static_cast<GLsizei>(textureCreateInfo.textureSizeWidth),
		static_cast<GLsizei>(textureCreateInfo.textureSizeHeight),
		GetOpenGlTextureFormat(textureCreateInfo.textureFormat), GetOpenglDataType(textureCreateInfo.dataType), textureCreateInfo.data
	);

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
			case Attachment::Color01:
			case Attachment::Color02:
			case Attachment::Color03:
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
	glBlitFramebuffer(
		src0Size.x, src0Size.y,
		src1Size.x, src1Size.y,
		target0Size.x, target0Size.y,
		target1Size.x, target1Size.y, GL_DEPTH_BUFFER_BIT, GetOpenglTextureFiltering(textureFiltering)
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

void Rhi::ComputeTextureWrapper(const uint32_t textureId, const TextureWrapping textureWrapping)
{
	switch (textureWrapping)
	{
		case TextureWrapping::Repeat:
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;
		
		case TextureWrapping::MirroredRepeat:
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			break;
		
		case TextureWrapping::ClampToEdge:
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			break;
		
		case TextureWrapping::ClampToBorder:
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			break;

		case TextureWrapping::None:
			break;
	}
}

void Rhi::ComputeOpenglTextureFilter(const uint32_t textureId, const TextureFiltering textureFilter)
{
	switch (textureFilter)
	{
		case TextureFiltering::Linear:
			glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
			
		case TextureFiltering::Nearest:
			glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
			
		case TextureFiltering::None:
			break;
	}
}

uint32_t Rhi::GetOpenglTextureFiltering(TextureFiltering textureFilter)
{
	switch (textureFilter)
	{
		case TextureFiltering::None:
			return GL_NONE;

		case TextureFiltering::Linear:
			return GL_LINEAR;

		case TextureFiltering::Nearest:
			return GL_NEAREST;
	}

	return GL_NONE;
}

uint32_t Rhi::TextureTypeToOpenglTexture(const TextureType textureType)
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
	if (!m_ShaderMap.contains(shaderId))
	{
		Logger::LogFatal("No shader with id #{}", shaderId);
		throw std::runtime_error("No shader with this id");
	}
}

int Rhi::GetUniformInMap(const uint32_t shaderId, const char* const uniformKey)
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

	Logger::LogDebug("---------------\n");
	Logger::LogDebug("Debug message ({}", id);
	Logger::LogDebug("): {}\n", message);
	
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
        	Logger::LogDebug("Source: API");
    		break;
    	
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        	Logger::LogDebug("Source: Window System");
    		break;
    	
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
        	Logger::LogDebug("Source: Shader Compiler");
    		break;
    	
        case GL_DEBUG_SOURCE_THIRD_PARTY:
        	Logger::LogDebug("Source: Third Party");
    		break;
    	
        case GL_DEBUG_SOURCE_APPLICATION:
        	Logger::LogDebug("Source: Application");
    		break;
    	
        case GL_DEBUG_SOURCE_OTHER:
        	Logger::LogDebug("Source: Other");
    		break;

    	default:
    		Logger::LogDebug("Source: Unknown [{}]", source);
    }

	Logger::LogDebug("\n");

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
        	Logger::LogDebug("Type: Error");
    		break;
    	
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			Logger::LogDebug("Type: Deprecated Behaviour");
    		break;
    	
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			Logger::LogDebug("Type: Undefined Behaviour"); 
    		break;
    	
        case GL_DEBUG_TYPE_PORTABILITY:
			Logger::LogDebug("Type: Portability"); 
    		break;
    	
        case GL_DEBUG_TYPE_PERFORMANCE:
			Logger::LogDebug("Type: Performance"); 
    		break;
    	
        case GL_DEBUG_TYPE_MARKER:
			Logger::LogDebug("Type: Marker"); 
    		break;
    	
        case GL_DEBUG_TYPE_PUSH_GROUP:
			Logger::LogDebug("Type: Push Group"); 
    		break;
    	
        case GL_DEBUG_TYPE_POP_GROUP:
			Logger::LogDebug("Type: Pop Group"); 
    		break;
    	
        case GL_DEBUG_TYPE_OTHER:
			Logger::LogDebug("Type: Other"); 
    		break;

    	default:
			Logger::LogDebug("Type: Unknown [{}]", type); 
    }

	Logger::LogDebug("\n");
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
        	Logger::LogDebug("Severity: high");
    		break;

        case GL_DEBUG_SEVERITY_MEDIUM:
        	Logger::LogDebug("Severity: medium");
    		break;

        case GL_DEBUG_SEVERITY_LOW:
        	Logger::LogDebug("Severity: low");
    		break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
        	Logger::LogDebug("Severity: notification");
    		break;

    	default:
        	Logger::LogDebug("Severity: Unknown [{}]", severity);
    }
	
	Logger::LogDebug("\n");
	
	Logger::LogDebug("\n");
}


void Rhi::Initialize()
{
	gladLoadGL();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
	glDebugMessageCallback(reinterpret_cast<GLDEBUGPROC>(OpenglDebugCallBack), nullptr);  // NOLINT(clang-diagnostic-cast-function-type-strict)
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
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

void Rhi::UpdateShadowMapingData(const ShadowMappingData& shadowMappingData)
{
	m_LightShadowMappingUniform->Update(sizeof(ShadowMappingData), 0, &shadowMappingData);
}
