#include "resource/shader.hpp"

#include <fstream>

#include "rendering/rhi.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void Shader::Load([[maybe_unused]] const uint8_t* buffer, [[maybe_unused]] const int64_t length) 
{
}

void Shader::Load(const File& vertexShader, const File& fragmentShader)
{
    std::string vertexCode;
    GetShaderCode(vertexShader.GetFilepath().generic_string().c_str(), &vertexCode);

    std::string fragmentCode;
    GetShaderCode(fragmentShader.GetFilepath().generic_string().c_str(), &fragmentCode);

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    std::vector<ShaderCode> shaderCodes(2);

    shaderCodes[0].shaderType = VERTEX;
    shaderCodes[0].shaderCode = vShaderCode;

    shaderCodes[1].shaderType = FRAGMENT;
    shaderCodes[1].shaderCode = fShaderCode;
    
    m_Id = RHI::CreateShader(shaderCodes);

    m_Loaded = true;
}

XNOR_ENGINE void Shader::Recompile(const File& vertexShader, const File& fragmentShader)
{
    RHI::DestroyShader(m_Id);

    Load(vertexShader, fragmentShader);
}

void Shader::Unload()
{
    return void();
}

void Shader::SetInt(const std::string& keyName, int value) const
{
    RHI::SetUniform(UniformType::INT,&value,m_Id,keyName.c_str());
}

void Shader::SetBool(const std::string& keyName, bool value) const
{
    RHI::SetUniform(UniformType::BOOL,&value,m_Id,keyName.c_str());
}

void Shader::SetFloat(const std::string& keyName, float value) const
{
    RHI::SetUniform(UniformType::FLOAT,&value,m_Id,keyName.c_str());
}

void Shader::SetVec3(const std::string& keyName, const Vector3& value) const
{
    RHI::SetUniform(UniformType::VEC3,&value,m_Id,keyName.c_str());
}

void Shader::SetVec4(const std::string& keyName, const Vector4& value) const
{
    RHI::SetUniform(UniformType::VEC4,&value,m_Id,keyName.c_str());
}

void Shader::SetMat4(const std::string& keyName, const Matrix& value) const
{
    RHI::SetUniform(UniformType::MAT4,&value,m_Id,keyName.c_str());
}

uint32_t Shader::GetId() const
{
    return  m_Id;
}

void Shader::Use() const
{
    RHI::UseShader(m_Id);
}

void Shader::UnUse() const
{
    RHI::UnUseShader();
}


void Shader::GetShaderCode(const char* shaderPath, std::string* shaderCode)
{
    if (!shaderCode || !shaderPath)
        throw std::runtime_error("Shader Path or Shader are nullptr");

    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        shaderFile.open(shaderPath);
        std::stringstream stream;
        stream << shaderFile.rdbuf();
        shaderFile.close();
        *shaderCode = stream.str();
    }
    catch (std::ifstream::failure& e)
    {
        const std::string what = e.what();
        std::string message = "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ:";
        message += what;
        message += '\n';
        Logger::LogError("%s", message.c_str());
    }
}


