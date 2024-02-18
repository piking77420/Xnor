#include "rendering/rhi.hpp"

#include <glad/glad.h>

#include "utils/logger.hpp"


using namespace XnorCore;

void RHI::SetPolyGoneMode(PolyGoneFace face, PolyGoneMode mode)
{
	glPolygonMode(face,GL_POINT + mode);
}

uint32_t RHI::CreateModel(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indicies)
{
	ModelInternal modelInternal;
	modelInternal.nbrOfVertex = static_cast<uint32_t>(vertices.size());
	modelInternal.nbrOfIndicies = static_cast<uint32_t>(indicies.size()); 
	
	glCreateVertexArrays(1,&modelInternal.vao);

	glCreateBuffers(1,&modelInternal.vbo);
	glCreateBuffers(1,&modelInternal.ebo);

	GLintptr offset =  static_cast<GLintptr>(vertices.size() * sizeof(Vertex));
	glNamedBufferData(modelInternal.vbo, offset,vertices.data(),GL_STATIC_DRAW);
	offset = static_cast<GLintptr>(indicies.size() * sizeof(uint32_t));
	glNamedBufferData(modelInternal.ebo, offset,indicies.data(),GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(modelInternal.vao,0);
	glVertexArrayAttribBinding(modelInternal.vao,0,0);
	glVertexArrayAttribFormat(modelInternal.vao,0,3,GL_FLOAT,GL_FALSE,0);

	glEnableVertexArrayAttrib(modelInternal.vao,1);
	glVertexArrayAttribBinding(modelInternal.vao,1,0);
	glVertexArrayAttribFormat(modelInternal.vao,1,3,GL_FLOAT,GL_FALSE,offsetof(Vertex,normal));

	glEnableVertexArrayAttrib(modelInternal.vao,2);
	glVertexArrayAttribBinding(modelInternal.vao,2,0);
	glVertexArrayAttribFormat(modelInternal.vao,2,2,GL_FLOAT,GL_FALSE,offsetof(Vertex,textureCoord));

	glVertexArrayVertexBuffer(modelInternal.vao,0,modelInternal.vbo,0,sizeof(Vertex));
	glVertexArrayElementBuffer(modelInternal.vao,modelInternal.ebo);
	
	uint32_t modelId = static_cast<uint32_t>(m_ModelMap.size());
	
	m_ModelMap.emplace(modelId,modelInternal);
	
	return modelId;
}

bool RHI::DestroyModel(uint32_t modelID)
{
	if(!m_ModelMap.contains(modelID))
	{
		return false;
	}

	const ModelInternal* model = &m_ModelMap.at(modelID);

	glDeleteBuffers(1,&model->vbo);
	glDeleteBuffers(1,&model->ebo);
	glDeleteVertexArrays(1,&model->vao);

	return true;
}

void RHI::DrawModel(uint32_t modelID)
{
	const ModelInternal model = m_ModelMap.at(modelID);
	glBindVertexArray(model.vao);
	glDrawElements(GL_TRIANGLES,  static_cast<GLsizei>(model.nbrOfIndicies), GL_UNSIGNED_INT, 0);
}

/*
void RHI::BindMaterial(const Material& material)
{
	material.shader->Use();
	
	
	
	material.shader->UnUse();
}*/

void RHI::DestroyShader(uint32_t shaderID)
{
	IsShaderValid(shaderID);
	
	glDeleteShader(shaderID);
}

void RHI::CheckCompilationError(uint32_t shaderId, const std::string& type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);
			Logger::LogError("Error while compiling shader of type %s: %s", type.c_str(), infoLog);
		}
	}
	else
	{
		glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderId, 1024, nullptr, infoLog);
			Logger::LogError("Error while linking shader program of type %s: %s", type.c_str(), infoLog);
		}
	}
}

uint32_t RHI::CreateShader(const std::vector<ShaderCode>& shaderCodes)
{
	uint32_t shaderID = glCreateProgram();
	
	std::vector<uint32_t> shadersID(shaderCodes.size());

	for (size_t i = 0; i < shaderCodes.size(); ++i)
	{
		shadersID[i] = glCreateShader(GetOpenglShaderType(shaderCodes[i].shaderType));
		glShaderSource(shadersID[i], 1, &shaderCodes[i].shaderCode, nullptr);
		glCompileShader(shadersID[i]);
		RHI::CheckCompilationError(shadersID[i], RHI::GetShaderTypeToString(shaderCodes[i].shaderType));
		glAttachShader(shaderID, shadersID[i]);

	}
	glLinkProgram(shaderID);
	RHI::CheckCompilationError(shaderID, "PROGRAM");
	m_ShaderMap.emplace(shaderID,ShaderInternal());

	return shaderID;
}

void RHI::UseShader(const uint32_t shaderID)
{
#ifdef _DEBUG
	IsShaderValid(shaderID);
#endif
#ifdef NDEBUG 

#endif // NDEBUG 
	glUseProgram(shaderID);
}

void RHI::UnUseShader()
{
	glUseProgram(0);
}

void RHI::SetUniform(UniformType uniformType, const void* data,uint32_t shaderID,
	const char* uniformKey)
{
	GLint uniformLocation = GetUniformInMap(shaderID,uniformKey);

	switch (uniformType)
	{
		case UniformType::INT:
			glUniform1i(uniformLocation, *reinterpret_cast<const int*>(data));
			break;
		case UniformType::BOOL:
			glUniform1i(uniformLocation, *reinterpret_cast<const bool*>(data));
			break;
		case UniformType::FLOAT:
			glUniform1f(uniformLocation, *reinterpret_cast<const bool*>(data));
			break;
		case UniformType::VEC3:
			glUniform3fv(uniformLocation,1, reinterpret_cast<const GLfloat*>(data));
			break;
		case UniformType::VEC4:
			glUniform4fv(uniformLocation,1, reinterpret_cast<const GLfloat*>(data));
			break;
		case UniformType::MAT3:
			glUniformMatrix3fv(uniformLocation,1,GL_FALSE, reinterpret_cast<const GLfloat*>(data));
			break;
		case UniformType::MAT4:
			glUniformMatrix4fv(uniformLocation,1,GL_FALSE, reinterpret_cast<const GLfloat*>(data));
			break;
	}

}

void RHI::CreateTexture(uint32_t* textureId,TextureCreateInfo textureCreateInfo)
{
	glCreateTextures(GL_TEXTURE_2D, 1, textureId);
	ComputeTextureWrapper(*textureId,textureCreateInfo.textureWrapping);
	ComputeOpenglTextureFilter(*textureId,textureCreateInfo.textureFiltering);

	glTextureStorage2D(*textureId, 1, GetOpenglFormatFromTextureFormat(textureCreateInfo.textureFormat),static_cast<GLsizei>(textureCreateInfo.textureSizeWidth), static_cast<GLsizei>(textureCreateInfo.textureSizeHeight));
	glTextureSubImage2D(*textureId, 0, 0, 0, static_cast<GLsizei>(textureCreateInfo.textureSizeWidth), static_cast<GLsizei>(textureCreateInfo.textureSizeHeight), GL_RGBA, GL_UNSIGNED_BYTE,nullptr);
	glGenerateTextureMipmap(*textureId);
	
}



void RHI::DestroyTexture(const uint32_t* textureId)
{
	glDeleteTextures(1,textureId);
}

void RHI::BindTexture(TextureType textureType, uint32_t textureID)
{
	glBindTextureUnit(TextureTypeToOpenglTexture(textureType), textureID);
}

void RHI::UnBindTexture(TextureType textureType)
{
	glBindTextureUnit(TextureTypeToOpenglTexture(textureType), 0);
}

void RHI::CreateFrameBuffer(uint32_t* frameBufferID)
{
	glCreateFramebuffers(1,frameBufferID);
}

void RHI::DestroyFrameBuffer(uint32_t* frameBufferID)
{
	glDeleteFramebuffers(1,frameBufferID);
}

uint32_t RHI::GetOpenglShaderType(ShaderType shaderType)
{
	switch (shaderType)
	{
		case VERTEX :
			return  GL_VERTEX_SHADER;
		case FRAGMENT :
			return  GL_FRAGMENT_SHADER;
		case GEOMETRY :
			return  GL_GEOMETRY_SHADER;
		case COMPUTE :
			return GL_COMPUTE_SHADER;
	}

	return  GL_VERTEX_SHADER;
}

std::string RHI::GetShaderTypeToString(ShaderType shaderType)
{
	switch (shaderType)
	{
	case VERTEX :
		return  "VERTEX_SHADER";
	case FRAGMENT :
		return  "FRAGMENT_SHADER";
	case GEOMETRY :
		return  "GEOMETRY_SHADER";
	case COMPUTE :
		return "COMPUTE_SHADER";
	}
	
	return  "UNKNOW_SHADER_TYPE";
}

void RHI::ComputeTextureWrapper(uint32_t textureID, TextureWrapping textureWrapping)
{
	switch (textureWrapping)
	{
	case TextureWrapping::REPEAT :
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case TextureWrapping::MIRRORED_REPEAT :
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;
	case TextureWrapping::CLAMP_TO_EDGE:
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	case TextureWrapping::CLAMP_TO_BORDER:
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		break;
	}

	
}


void RHI::ComputeOpenglTextureFilter(uint32_t textureID, TextureFiltering textureFilter)
{
	switch (textureFilter)
	{
	case TextureFiltering::LINEAR:
		glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case TextureFiltering::NEAREST:
		glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	}
}

uint32_t RHI::TextureTypeToOpenglTexture(TextureType textureType)
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

uint32_t RHI::GetOpenglFormatFromTextureFormat(TextureFormat textureFormat)
{
	switch (textureFormat)
	{
	case TextureFormat::R_8:
		return GL_R8;
	case TextureFormat::R_16:
		return GL_R16;
	case TextureFormat::RG_8:
		return GL_RG8;
	case TextureFormat::RG_16:
		return GL_RG16;
	case TextureFormat::RGB_8:
		return GL_RGB8;
	case TextureFormat::RGB_16:
		return GL_RGB16;
	case TextureFormat::RGBA_8:
		return GL_RGBA8;
	case TextureFormat::RGBA_16:
		return GL_RGBA16;
	case TextureFormat::R_16F:
		return GL_R16F;
	case TextureFormat::RG_16F:
		return GL_RG16F;
	case TextureFormat::RGB_16F:
		return GL_RGB16F;
	case TextureFormat::RGBA_16F:
		return GL_RGBA16F;
	case TextureFormat::DEPTH_COMPONENT:
		return GL_DEPTH_COMPONENT;
	case TextureFormat::RED:
		return GL_RED;
	case TextureFormat::RGB:
		return GL_RGB;
	case TextureFormat::RGBA:
		return GL_RGBA;
	}
	return GL_RGB;
}

void RHI::IsShaderValid(uint32_t shaderID)
{
	if (!m_ShaderMap.contains(shaderID))
	{
		Logger::LogFatal("No shader with this id %d",shaderID);
		throw std::runtime_error("No shader With this Id");
	}
}



int RHI::GetUniformInMap(uint32_t shaderID, const char* uniformKey)
{
	std::map<std::string,uint32_t>& shaderUniformMap = m_ShaderMap.at(shaderID).uniformMap;

	if (shaderUniformMap.find(uniformKey) != shaderUniformMap.end())
	{
		return static_cast<int>(shaderUniformMap[uniformKey]);
	}

	GLint location = glGetUniformLocation(shaderID, uniformKey);
	if(location == NULL_UNIFORM_LOCATION)
	{
		Logger::LogWarning("There is no uniform with this key [%s] in the shader %d",uniformKey,shaderID);
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
	for (std::unordered_map<uint32_t,ModelInternal>::iterator it = m_ModelMap.begin() ; it != m_ModelMap.end(); it++)
	{
		DestroyModel(it->first);
	}
	delete m_CameraUniform;
	delete m_ModelUniform;
}

void RHI::PrepareUniform()
{
	m_CameraUniform = new UniformBuffer();
	m_CameraUniform->Allocate(sizeof (CameraUniformData), nullptr);
	m_CameraUniform->Bind(0);


	m_ModelUniform = new UniformBuffer();
	m_ModelUniform->Allocate(sizeof(ModelUniformData), nullptr);
	m_ModelUniform->Bind(1);
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


