#include "rendering/rhi.hpp"

#include <glad/glad.h>

#include "utils/logger.hpp"

using namespace XnorCore;

void RHI::SetPolyGoneMode(const PolygonFace face, const PolygonMode mode)
{
	glPolygonMode(static_cast<GLenum>(face), GL_POINT + static_cast<GLenum>(mode));
}

uint32_t RHI::CreateModel(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indicies)
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
	
	const uint32_t modelId = static_cast<uint32_t>(m_ModelMap.size());
	
	m_ModelMap.emplace(modelId, modelInternal);
	
	return modelId;
}

bool RHI::DestroyModel(const uint32_t modelId)
{
	if (!m_ModelMap.contains(modelId))
		return false;

	const ModelInternal* model = &m_ModelMap.at(modelId);

	glDeleteBuffers(1, &model->vbo);
	glDeleteBuffers(1, &model->ebo);
	glDeleteVertexArrays(1, &model->vao);

	return true;
}

void RHI::DrawModel(const uint32_t modelId)
{
	const ModelInternal model = m_ModelMap.at(modelId);
	glBindVertexArray(model.vao);
	glDrawElements(GL_TRIANGLES,  static_cast<GLsizei>(model.nbrOfIndicies), GL_UNSIGNED_INT, nullptr);
}

/*
void RHI::BindMaterial(const Material& material)
{
	material.shader->Use();
	
	
	
	material.shader->UnUse();
}*/

void RHI::DestroyShader(const uint32_t shaderId)
{
	IsShaderValid(shaderId);
	
	glDeleteShader(shaderId);
}

void RHI::CheckCompilationError(const uint32_t shaderId, const std::string& type)
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

uint32_t RHI::CreateShader(const std::vector<ShaderCode>& shaderCodes)
{
	uint32_t shaderId = glCreateProgram();
	
	std::vector<uint32_t> shadersId(shaderCodes.size());
	
	for (size_t i = 0; i < shaderCodes.size(); i++)
	{
		shadersId[i] = glCreateShader(GetOpenglShaderType(shaderCodes[i].shaderType));
		glShaderSource(shadersId[i], 1, &shaderCodes[i].shaderCode, nullptr);
		glCompileShader(shadersId[i]);
		CheckCompilationError(shadersId[i], GetShaderTypeToString(shaderCodes[i].shaderType));
		glAttachShader(shaderId, shadersId[i]);
	}
	
	glLinkProgram(shaderId);
	CheckCompilationError(shaderId, "PROGRAM");
	m_ShaderMap.emplace(shaderId,ShaderInternal());

	return shaderId;
}

void RHI::UseShader(const uint32_t shaderId)
{
#ifdef _DEBUG
	IsShaderValid(shaderId);
#endif
#ifdef NDEBUG 

#endif // NDEBUG 
	glUseProgram(shaderId);
}

void RHI::UnUseShader()
{
	glUseProgram(0);
}

void RHI::SetUniform(const UniformType uniformType, const void* data, const uint32_t shaderId, const char* uniformKey)
{
	const GLint uniformLocation = GetUniformInMap(shaderId, uniformKey);

	switch (uniformType)
	{
		case UniformType::INT:
			glUniform1i(uniformLocation, *static_cast<const int32_t*>(data));
			break;
			
		case UniformType::BOOL:
			glUniform1i(uniformLocation, *static_cast<const bool*>(data));
			break;
			
		case UniformType::FLOAT:
			glUniform1f(uniformLocation, *static_cast<const GLfloat*>(data));
			break;
			
		case UniformType::VEC3:
			glUniform3fv(uniformLocation, 1, static_cast<const GLfloat*>(data));
			break;
			
		case UniformType::VEC4:
			glUniform4fv(uniformLocation, 1, static_cast<const GLfloat*>(data));
			break;
			
		case UniformType::MAT3:
			glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, static_cast<const GLfloat*>(data));
			break;
			
		case UniformType::MAT4:
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, static_cast<const GLfloat*>(data));
			break;
	}
}

void RHI::CreateTexture(uint32_t* const textureId, const TextureCreateInfo& textureCreateInfo)
{
	glCreateTextures(GL_TEXTURE_2D, 1, textureId);

	ComputeTextureWrapper(*textureId,textureCreateInfo.textureWrapping);
	ComputeOpenglTextureFilter(*textureId,textureCreateInfo.textureFiltering);

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
		GetOpenGlTextureFormat(textureCreateInfo.textureFormat), GL_UNSIGNED_BYTE, textureCreateInfo.data
	);

	glGenerateTextureMipmap(*textureId);
}

void RHI::DestroyTexture(const uint32_t* textureId)
{
	glDeleteTextures(1,textureId);
}

void RHI::BindTexture(const uint32_t unit, const uint32_t textureId)
{
	glBindTextureUnit(unit, textureId);
}

void RHI::UnBindTexture(const TextureType textureType)
{
	glBindTextureUnit(TextureTypeToOpenglTexture(textureType), 0);
}

void RHI::CreateFrameBuffer(uint32_t* const frameBufferId, const uint32_t renderPassId,const std::vector<Texture*>& targets)
{
	
	if (!m_RenderPassMap.contains(renderPassId))
		Logger::LogError("There is no renderPass with this id");
	
	glCreateFramebuffers(1, frameBufferId);
	const RenderPassIternal& renderPassIternal = m_RenderPassMap.at(renderPassId);

	for (size_t i = 0; i < renderPassIternal.AttachementsType.size(); i++)
	{
		uint32_t openglAttachment = 0;
		switch (renderPassIternal.AttachementsType[i])
		{
			case AttachementsType::Color:
			case AttachementsType::Position:
			case AttachementsType::Normal:
			case AttachementsType::Texturecoord:
				openglAttachment = static_cast<const uint32_t>(GL_COLOR_ATTACHMENT0 + i);
				break;
			
			case AttachementsType::Depth:
				openglAttachment = GL_DEPTH_ATTACHMENT;
				break;
			
			case AttachementsType::Stencil:
				openglAttachment = GL_STENCIL_ATTACHMENT;
				break;
			
			case AttachementsType::DepthAndStencil:
				openglAttachment = GL_DEPTH_STENCIL_ATTACHMENT;
				break;
		}
		
		glNamedFramebufferTexture(*frameBufferId, openglAttachment,targets.at(i)->GetId(), 0);
	}
}


void RHI::DestroyFrameBuffer(const uint32_t* const frameBufferId)
{
	if (glIsFramebuffer(*frameBufferId))
		glDeleteFramebuffers(1, frameBufferId);
}

void RHI::BindFrameBuffer(const uint32_t frameBufferId)
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
}

void RHI::UnBindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*
void RHI::AttachColorAttachementToFrameBuffer(const uint32_t frameBufferId, const uint32_t attachmentIndex, const uint32_t textureId)
{
	glNamedFramebufferTexture(frameBufferId, GL_COLOR_ATTACHMENT0 + attachmentIndex,textureId, 0);
}*/

void RHI::CreateRenderPass(uint32_t* const renderPassId, const std::vector<AttachementsType>& attachementsType)
{
	*renderPassId = static_cast<uint32_t>(m_RenderPassMap.size());
	m_RenderPassMap.emplace(*renderPassId, attachementsType);
}


uint32_t RHI::GetOpenglShaderType(const ShaderType shaderType)
{
	switch (shaderType)
	{
		case ShaderType::VERTEX:
			return GL_VERTEX_SHADER;
			
		case ShaderType::FRAGMENT:
			return GL_FRAGMENT_SHADER;
			
		case ShaderType::GEOMETRY:
			return GL_GEOMETRY_SHADER;
			
		case ShaderType::COMPUTE:
			return GL_COMPUTE_SHADER;
	}

	return GL_VERTEX_SHADER;
}

std::string RHI::GetShaderTypeToString(const ShaderType shaderType)
{
	switch (shaderType)
	{
		case ShaderType::VERTEX:
			return "VERTEX_SHADER";

		case ShaderType::FRAGMENT:
			return "FRAGMENT_SHADER";

		case ShaderType::GEOMETRY:
			return "GEOMETRY_SHADER";

		case ShaderType::COMPUTE:
			return "COMPUTE_SHADER";
	}
	
	return"UNKNOWN_SHADER_TYPE";
}

void RHI::ComputeTextureWrapper(const uint32_t textureId, const TextureWrapping textureWrapping)
{
	switch (textureWrapping)
	{
		case TextureWrapping::REPEAT:
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;
		
		case TextureWrapping::MIRRORED_REPEAT:
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			break;
		
		case TextureWrapping::CLAMP_TO_EDGE:
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			break;
		
		case TextureWrapping::CLAMP_TO_BORDER:
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			break;

		case TextureWrapping::NONE:
			break;
	}
}

void RHI::ComputeOpenglTextureFilter(const uint32_t textureId, const TextureFiltering textureFilter)
{
	switch (textureFilter)
	{
		case TextureFiltering::LINEAR:
			glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
			
		case TextureFiltering::NEAREST:
			glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
			
		case TextureFiltering::NONE:
			break;
	}
}

uint32_t RHI::TextureTypeToOpenglTexture(const TextureType textureType)
{
	switch (textureType)
	{
		case TextureType::TEXTURE_1D:
			return GL_TEXTURE_1D;

		case TextureType::TEXTURE_2D:
			return GL_TEXTURE_2D;

		case TextureType::TEXTURE_3D:
			return GL_TEXTURE_3D;

		case TextureType::TEXTURE_1D_ARRAY:
			return GL_TEXTURE_1D_ARRAY;

		case TextureType::TEXTURE_2D_ARRAY:
			return GL_TEXTURE_2D_ARRAY;

		case TextureType::TEXTURE_RECTANGLE:
			return GL_TEXTURE_RECTANGLE;

		case TextureType::TEXTURE_CUBE_MAP:
			return GL_TEXTURE_CUBE_MAP;

		case TextureType::TEXTURE_CUBE_MAP_ARRAY:
			return GL_TEXTURE_CUBE_MAP_ARRAY;

		case TextureType::TEXTURE_BUFFER:
			return GL_TEXTURE_BUFFER;

		case TextureType::TEXTURE_2D_MULTISAMPLE:
			return GL_TEXTURE_2D_MULTISAMPLE;

		case TextureType::TEXTURE_2D_MULTISAMPLE_ARRAY:
			return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
	}

	return 0;
}

uint32_t RHI::GetOpenglInternalFormat(const TextureInternalFormat textureFormat)
{
	switch (textureFormat)
	{
		case TextureInternalFormat::R_8:
			return GL_R8;

		case TextureInternalFormat::R_16:
			return GL_R16;

		case TextureInternalFormat::RG_8:
			return GL_RG8;

		case TextureInternalFormat::RG_16:
			return GL_RG16;

		case TextureInternalFormat::RGB_8:
			return GL_RGB8;

		case TextureInternalFormat::RGB_16:
			return GL_RGB16;

		case TextureInternalFormat::RGBA_8:
			return GL_RGBA8;

		case TextureInternalFormat::RGBA_16:
			return GL_RGBA16;

		case TextureInternalFormat::R_16F:
			return GL_R16F;

		case TextureInternalFormat::RG_16F:
			return GL_RG16F;

		case TextureInternalFormat::RGB_16F:
			return GL_RGB16F;

		case TextureInternalFormat::RGBA_16F:
			return GL_RGBA16F;

		case TextureInternalFormat::DEPTH_COMPONENT:
			return GL_DEPTH_COMPONENT;
		
		case TextureInternalFormat::DEPTH_STENCIL:
			return GL_DEPTH24_STENCIL8;
	}

	Logger::LogError("Texture InternalFormat not supported, defaulting to RGB");
	
	return GL_RGB;
}

uint32_t RHI::GetOpenGlTextureFormat(const TextureFormat textureFormat)
{
	switch (textureFormat)
	{
		case TextureFormat::RED: 
			return GL_RED;
	
		case TextureFormat::RGB:
			return GL_RGB;
	
		case TextureFormat::RGBA:
			return GL_RGBA;
	}
	
	return GL_RGB;
}

void RHI::IsShaderValid(const uint32_t shaderId)
{
	if (!m_ShaderMap.contains(shaderId))
	{
		Logger::LogFatal("No shader with id #{}", shaderId);
		throw std::runtime_error("No shader with this id");
	}
}

int RHI::GetUniformInMap(uint32_t shaderId, const char* uniformKey)
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

RHI::RHI()
{
	gladLoadGL();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

RHI::~RHI()
{
	for (std::unordered_map<uint32_t, ModelInternal>::iterator it = m_ModelMap.begin() ; it != m_ModelMap.end(); it++)
	{
		DestroyModel(it->first);
	}

	delete m_CameraUniform;
	delete m_ModelUniform;
	delete m_LightUniform;
}

void RHI::PrepareUniform()
{
	m_CameraUniform = new UniformBuffer;
	m_CameraUniform->Allocate(sizeof(CameraUniformData), nullptr);
	m_CameraUniform->Bind(0);
	
	m_ModelUniform = new UniformBuffer;
	m_ModelUniform->Allocate(sizeof(ModelUniformData), nullptr);
	m_ModelUniform->Bind(1);
	
	m_LightUniform = new UniformBuffer;
	m_LightUniform->Allocate(sizeof(GpuLightData),nullptr);
	m_LightUniform->Bind(2);
}

void RHI::SetClearColor(const Vector4& color) const
{
	glClearColor(color.x, color.y, color.z, color.w);
}

void RHI::ClearColorAndDepth() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RHI::UpdateModelUniform(const ModelUniformData& modelUniformData) const
{	
	constexpr size_t size = sizeof(ModelUniformData);
	m_ModelUniform->Update(size,0,modelUniformData.model.Raw());
}

void RHI::UpdateCameraUniform(const CameraUniformData& cameraUniformData) const
{
	m_CameraUniform->Update(sizeof(CameraUniformData),0,cameraUniformData.view.Raw());
}

void RHI::UpdateLight(const GpuLightData& lightData) const
{
	m_LightUniform->Update(sizeof(GpuLightData),0,&lightData.nbrOfPointLight);
}
