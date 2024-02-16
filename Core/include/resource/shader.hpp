#pragma once

#include "core.hpp"
#include "resource/resource.hpp"

BEGIN_XNOR_CORE

class Shader : public Resource
{
public:
	XNOR_ENGINE void Load(const uint8_t* buffer, int64_t length) override;

	XNOR_ENGINE void Load(const File& vertexShader, const File& fragmentShader);

	XNOR_ENGINE void Recompile(const File& vertexShader, File& fragmentShader);

	XNOR_ENGINE void Unload() override;

	virtual void Bind();

	virtual void UnBind();

private:
	static void GetShaderCode(const char* shaderPath, std::string* shaderCode);

	// Move to rhi
	void ShaderCompilationError(std::uint32_t shaderId, const std::string& type);

	uint32_t m_Id = 0;
};

END_XNOR_CORE
