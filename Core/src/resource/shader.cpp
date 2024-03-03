#include "resource/shader.hpp"

#include "rendering/rhi.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

ShaderType Shader::FileExtensionToType(const std::string& extension)
{
    if (std::ranges::find(VertexFileExtensions, extension) != VertexFileExtensions.end())
        return ShaderType::Vertex;
    if (std::ranges::find(FragmentFileExtensions, extension) != FragmentFileExtensions.end())
        return ShaderType::Fragment;
    if (std::ranges::find(GeometryFileExtensions, extension) != GeometryFileExtensions.end())
        return ShaderType::Geometry;
    if (std::ranges::find(ComputeFileExtensions, extension) != ComputeFileExtensions.end())
        return ShaderType::Compute;

    throw std::invalid_argument("Invalid file extension for shader");
}

void Shader::Load(const Pointer<File>& shader)
{
    ShaderType&& type = FileExtensionToType(shader->GetExtension());
    Load(shader->GetData(), shader->GetSize(), type);

    m_Files[static_cast<size_t>(type)] = shader;
    m_Loaded = true;
}

void Shader::Load(const uint8_t*, int64_t)
{
    throw std::runtime_error("Cannot load shader without shader type information");
}

void Shader::Load(const char_t* buffer, const int64_t length, const ShaderType type)
{
    ShaderCode& code = m_Code[static_cast<size_t>(type)];
    code.code = buffer;
    code.codeLength = static_cast<int32_t>(length);
    code.type = type;
    m_Loaded = true;
}

void Shader::CreateInRhi()
{
    std::vector<ShaderCode> code(m_Code.size());
    std::ranges::copy(m_Code, code.begin());
    m_Id = Rhi::CreateShaders(code, {m_DepthFunction,m_BlendFunction});
    m_LoadedInRhi = true;
   
}

void Shader::DestroyInRhi()
{
    Rhi::DestroyProgram(m_Id);
    m_Id = 0;
    m_LoadedInRhi = false;
}

void Shader::Recompile()
{
    DestroyInRhi();

    for (auto&& file : m_Files)
    {
        if (file)
            file->Reload();
    }

    CreateInRhi();
}

void Shader::Unload()
{
    m_Files.fill(nullptr);
    m_Code.fill({});
}

void Shader::SetInt(const std::string& keyName, const int value) const
{
    Rhi::SetUniform(UniformType::Int, &value, m_Id, keyName.c_str());
}

void Shader::SetBool(const std::string& keyName, const bool value) const
{
    Rhi::SetUniform(UniformType::Bool, &value, m_Id, keyName.c_str());
}

void Shader::SetFloat(const std::string& keyName, const float value) const
{
    Rhi::SetUniform(UniformType::Float, &value, m_Id, keyName.c_str());
}

void Shader::SetVec3(const std::string& keyName, const Vector3& value) const
{
    Rhi::SetUniform(UniformType::Vec3, &value, m_Id, keyName.c_str());
}

void Shader::SetVec4(const std::string& keyName, const Vector4& value) const
{
    Rhi::SetUniform(UniformType::Vec4, &value, m_Id, keyName.c_str());
}

void Shader::SetMat4(const std::string& keyName, const Matrix& value) const
{
    Rhi::SetUniform(UniformType::Mat4, &value, m_Id, keyName.c_str());
}

uint32_t Shader::GetId() const
{
    return m_Id;
}

void Shader::Use() const
{
    Rhi::UseShader(m_Id);
}

void Shader::Unuse() const
{
    Rhi::UnuseShader();
}

void Shader::SetDepthFunction(DepthFunction depthFunction)
{
    if(m_LoadedInRhi)
    {
        Logger::LogError("You are trying do modifies depth function for already initialize shader !\n Shader id = {}",m_Id);
        return;
    }
    m_DepthFunction = depthFunction;
}

void Shader::SetBlendFunction(BlendFunction blendFunction)
{
    if(m_LoadedInRhi)
    {
        Logger::LogError("You are trying do modifies blend function for already initialize shader !\n Shader id = {}",m_Id);
        return;
    }
    m_BlendFunction = blendFunction;
}
