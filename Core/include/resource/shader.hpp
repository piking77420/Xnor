#pragma once

#include "core.hpp"
#include "resource/resource.hpp"

BEGIN_XNOR_CORE

class Shader : public Resource
{
private:
	static void GetShaderCode(const char* shaderPath, std::string* shaderCode);
public:
	XNOR_ENGINE void Load(const uint8_t* buffer, int64_t length) override;

	XNOR_ENGINE void Load(const File& vertexShader, const File& fragmentShader);

	XNOR_ENGINE void Recompile(const File& vertexShader, const File& fragmentShader);

	XNOR_ENGINE void Unload() override;

	uint32_t GetId() const;

private:
	uint32_t m_Id = 0;
};

END_XNOR_CORE
