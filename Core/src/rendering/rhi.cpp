#include "rendering/rhi.hpp"
#include <glad/glad.h>
#include "resource/shader.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void RHI::SetPolyGoneMode(PolyGoneFace face, PolyGoneMode mode)
{
	glPolygonMode(face + GL_FRONT_LEFT,GL_POINT + mode);
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

void RHI::BindMaterial(const Material& material)
{
	// TO DO 
	
	glUseProgram(material.shader->GetId());

	
	
}

void RHI::DestroyShader(uint32_t id)
{
	glDeleteShader(id);
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
		shadersID[i] = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(shadersID[i], 1, &shaderCodes[i].shaderCode, nullptr);
		glCompileShader(shadersID[i]);
		RHI::CheckCompilationError(shadersID[i], RHI::GetShaderTypeToString(shaderCodes[i].shaderType));
		glAttachShader(shaderID, shadersID[i]);

	}
	glLinkProgram(shaderID);
	RHI::CheckCompilationError(shaderID, "PROGRAM");

	return shaderID;
}

void RHI::UseShader(const uint32_t shaderID)
{
	glUseProgram(shaderID);
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
}



void RHI::SetClearColor(const Vector4& color) const
{
	glClearColor(color.x, color.y, color.z, color.w);
}

void RHI::ClearColorAndDepth() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


