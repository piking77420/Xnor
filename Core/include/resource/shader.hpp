#pragma once

#include "core.hpp"
#include "resource/resource.hpp"

BEGIN_XNOR_CORE

class Shader : public Resource
{
public:

	virtual XNOR_ENGINE void Load(const uint8_t* buffer, int64_t length) override;

	virtual XNOR_ENGINE void Load(File& file) override;

	XNOR_ENGINE void Load(File& vertexShader,File& fragmentShader);

	XNOR_ENGINE void Recompile(File& vertexShader, File& fragmentShader);

	virtual XNOR_ENGINE void Unload() override;

	virtual void Bind();

	virtual void UnBind();

private:

	static void GetShaderCode(const char* shaderPath,std::string* buffer);

	// Move to rhi
	void ShaderCompilationError(std::uint32_t shaderID, std::string type);

	uint32_t m_Id;
};

END_XNOR_CORE