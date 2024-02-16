#include "rendering/rhi.hpp"
#include <glad/glad.h>
#include "resource/shader.hpp"

using namespace XnorCore;

void RHI::SetPolyGoneMode(PolyGoneFace face, PolyGoneMode mode)
{
	glPolygonMode(face + GL_FRONT_LEFT,GL_POINT + mode);
}

uint32_t RHI::CreateModel(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indicies)
{
	ModelInternal modelInternal;
	modelInternal.nbrOfVertex = vertices.size();
	modelInternal.nbrOfIndicies = indicies.size(); 
	
	glCreateVertexArrays(1,&modelInternal.vao);

	glCreateBuffers(1,&modelInternal.vbo);
	glCreateBuffers(1,&modelInternal.ebo);

	glNamedBufferData(modelInternal.vbo,sizeof(Vertex) * vertices.size(),vertices.data(),GL_STATIC_DRAW);
	glNamedBufferData(modelInternal.ebo,sizeof(uint32_t) * indicies.size() ,indicies.data(),GL_STATIC_DRAW);

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
	
	uint32_t modelId = m_ModelMap.size();
	
	m_ModelMap.emplace(modelId,modelInternal);
	
	return modelId;
}

bool RHI::DestroyModel(uint32_t modelID)
{
	if(!m_ModelMap.contains(modelID))
	{
		return false;
	}
	
	ModelInternal* model = &m_ModelMap.at(modelID);

	glDeleteBuffers(1,&model->vbo);
	glDeleteBuffers(1,&model->ebo);
	glDeleteVertexArrays(1,&model->vao);

	return true;
}

void RHI::DrawModel(uint32_t modelID)
{
	ModelInternal model = m_ModelMap.at(modelID);
	glBindVertexArray(model.vao);
	glDrawElements(GL_TRIANGLES, model.nbrOfIndicies, GL_UNSIGNED_INT, 0);
}

void RHI::BindMaterial(const Material& material)
{
	glUseProgram(material.shader->GetId());

	
	
}

RHI::RHI()
{
	gladLoadGL();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

RHI::~RHI()
{
}



void RHI::SetClearColor(const Vector4& color) const
{
	glClearColor(color.x, color.y, color.z, color.w);
}

void RHI::ClearColorAndDepth() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


