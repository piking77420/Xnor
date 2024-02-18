#pragma once

#include "core.hpp"
#include "Maths/math.hpp"
#include "resource/resource.hpp"

BEGIN_XNOR_CORE

class Shader : public Resource
{
private:
	static void GetShaderCode(const char* shaderPath, std::string* shaderCode);
public:
	XNOR_ENGINE void Load(const File& vertexShader, const File& fragmentShader);
	
	XNOR_ENGINE void Load(const uint8_t* buffer, int64_t length) override;
	
	XNOR_ENGINE void Recompile(const File& vertexShader, const File& fragmentShader);

	XNOR_ENGINE void Unload() override;

	XNOR_ENGINE void SetInt(const std::string& keyName,int value) const ;
	
	XNOR_ENGINE void SetBool(const std::string& keyName,bool value) const ;
	
	XNOR_ENGINE void SetFloat(const std::string& keyName,float value) const ;
	
	XNOR_ENGINE void SetVec3(const std::string& keyName,const Vector3& value) const ;
	
	XNOR_ENGINE void SetVec4(const std::string& keyName,const Vector4& value) const ;
	
	XNOR_ENGINE void SetMat4(const std::string& keyName,const Matrix& value) const ;
	
	uint32_t GetId() const;

	void Use() const;

	void UnUse() const;

private:
	uint32_t m_Id = 0;
};




END_XNOR_CORE
