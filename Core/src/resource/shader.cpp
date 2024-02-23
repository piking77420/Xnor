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
    GetShaderCode(vertexShader.GetPath().generic_string().c_str(), &vertexCode);

    std::string fragmentCode;
    GetShaderCode(fragmentShader.GetPath().generic_string().c_str(), &fragmentCode);

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    std::vector<ShaderCode> shaderCodes(2);

    shaderCodes[0].shaderType = ShaderType::Vertex;
    shaderCodes[0].shaderCode = vShaderCode;

    shaderCodes[1].shaderType = ShaderType::Fragment;
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
}

void Shader::SetInt(const std::string& keyName, int value) const
{
    RHI::SetUniform(UniformType::Int,&value,m_Id,keyName.c_str());
}

void Shader::SetBool(const std::string& keyName, bool value) const
{
    RHI::SetUniform(UniformType::Bool,&value,m_Id,keyName.c_str());
}

void Shader::SetFloat(const std::string& keyName, float value) const
{
    RHI::SetUniform(UniformType::Float,&value,m_Id,keyName.c_str());
}

void Shader::SetVec3(const std::string& keyName, const Vector3& value) const
{
    RHI::SetUniform(UniformType::Vec3,&value,m_Id,keyName.c_str());
}

void Shader::SetVec4(const std::string& keyName, const Vector4& value) const
{
    RHI::SetUniform(UniformType::Vec4,&value,m_Id,keyName.c_str());
}

void Shader::SetMat4(const std::string& keyName, const Matrix& value) const
{
    RHI::SetUniform(UniformType::Mat4,&value,m_Id,keyName.c_str());
}

uint32_t Shader::GetId() const
{
    return m_Id;
}

void Shader::Use() const
{
    RHI::UseShader(m_Id);
}

void Shader::UnUse() const
{
    RHI::UnuseShader();
}


void Shader::GetShaderCode(const char* shaderPath, std::string* shaderCode)
{
    if (!shaderCode || !shaderPath)
        throw std::runtime_error("Shader path or code are invalid");

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
    catch (const std::ifstream::failure& e)
    {
        Logger::LogError("An error occured while attempting to read shader file {}: {}", shaderPath, e.what());
    }
}
