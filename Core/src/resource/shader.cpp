#include "resource/shader.hpp"

#include <fstream>
#include <glad/glad.h>

#include "utils/logger.hpp"

using namespace XnorCore;

void Shader::Load(const uint8_t* buffer, int64_t length) 
{
    throw std::runtime_error("Not implemented");
}

void Shader::Load(File& file)
{
    throw std::runtime_error("Not implemented");
}

void Shader::Load(const File& vertexShader, File& fragmentShader)
{
    std::string vertexCode;
    GetShaderCode(vertexShader.GetFilepath().generic_string().c_str(), &vertexCode);

    std::string fragmentCode;
    GetShaderCode(vertexShader.GetFilepath().generic_string().c_str(), &fragmentCode);

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Move to rhi
    const uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    ShaderCompilationError(vertex, "VERTEX");

    // Move to rhi
    const uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    ShaderCompilationError(fragment, "FRAGMENT");

    // Move to rhi
    m_Id = glCreateProgram();
    glAttachShader(m_Id, vertex);
    glAttachShader(m_Id, fragment);
    glLinkProgram(m_Id);
    Shader::ShaderCompilationError(m_Id, "PROGRAM");
}

XNOR_ENGINE void Shader::Recompile(const File& vertexShader, File& fragmentShader)
{
    // Move to rhi
    glDeleteShader(m_Id);

    Load(vertexShader, fragmentShader);
}

void Shader::Unload()
{
    return void();
}

void Shader::Bind()
{
    // Move to rhi
    glUseProgram(m_Id);
}

void Shader::UnBind()
{
    // Move to rhi
    glUseProgram(0);
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
        const std::string ewhat = e.what();
        std::string message = "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ:";
        message += ewhat;
        message += '\n';
       Logger::LogError("%s", message.c_str());
    }
}

void Shader::ShaderCompilationError(const uint32_t shaderId, const std::string& type)
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
