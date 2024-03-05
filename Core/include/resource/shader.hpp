#pragma once

#include <array>

#include "core.hpp"
#include "file/file.hpp"
#include "Maths/math.hpp"
#include "rendering/rhi_typedef.hpp"
#include "resource/resource.hpp"
#include "utils/pointer.hpp"

/// @file shader.hpp
/// @brief Defines the Shader class

BEGIN_XNOR_CORE

/// @brief Encapsulates a GPU shader
class Shader : public Resource
{
public:
	/// @brief Allowed extensions for vertex shaders
	XNOR_ENGINE static inline constexpr std::array<const char_t*, 3> VertexFileExtensions
	{
		".vert",
		".vertex",
		".vs"
	};

	/// @brief Allowed extensions for fragment shaders
	XNOR_ENGINE static inline constexpr std::array<const char_t*, 3> FragmentFileExtensions
	{
		".frag",
		".fragment",
		".fs"
	};

	/// @brief Allowed extensions for geometry shaders
	XNOR_ENGINE static inline constexpr std::array<const char_t*, 2> GeometryFileExtensions
	{
		".geom",
		".geometry"
	};

	/// @brief Allowed extensions for compute shaders
	XNOR_ENGINE static inline constexpr std::array<const char_t*, 2> ComputeFileExtensions
	{
		".comp",
		".compute"
	};

	/// @brief Gets the shader type via a file extension
	/// @param extension File extension
	/// @return @ref ShaderType type
	/// @throw std::invalid_argument If the extension isn't valid
	[[nodiscard]]
	static ShaderType FileExtensionToType(const std::string& extension);

	using Resource::Resource;

	using Resource::Load;

	/// @brief Loads a single shader file
	/// @param shader File
	/// @return Whether the load succeeded
	XNOR_ENGINE bool_t Load(const Pointer<File>& shader) override;

	/// @brief Loads raw shader code
	/// @param buffer Raw data
	/// @param length Raw data length
	/// @param type Shader type
	XNOR_ENGINE bool_t Load(const char_t* buffer, int64_t length, ShaderType type);

	/// @brief Creates the shader in the @ref Rhi
	XNOR_ENGINE void CreateInRhi() override;

	/// @brief Destroys the shader in the @ref Rhi
	XNOR_ENGINE void DestroyInRhi() override;

	/// @brief Recompiles the shader
	XNOR_ENGINE void Recompile();

	/// @brief Unloads the shader
	XNOR_ENGINE void Unload() override;

	/// @brief Sets an int (signed, 32 bits) variable in a shader
	/// @param keyName Variable name
	/// @param value Value
	XNOR_ENGINE void SetInt(const std::string& keyName, int32_t value) const;

	/// @brief Sets an bool (signed, 32 bits) variable in a shader
	/// @param keyName Variable name
	/// @param value Value
	XNOR_ENGINE void SetBool(const std::string& keyName, bool_t value) const;

	/// @brief Sets an float (32 bits) variable in a shader
	/// @param keyName Variable name
	/// @param value Value
	XNOR_ENGINE void SetFloat(const std::string& keyName, float_t value) const;

	/// @brief Sets a @ref Vector3 (3 float, 96 bits) variable in a shader
	/// @param keyName Variable name
	/// @param value Value
	XNOR_ENGINE void SetVec3(const std::string& keyName, const Vector3& value) const;

	/// @brief Sets a @ref Vector4 (4 float, 128 bits) variable in a shader
	/// @param keyName Variable name
	/// @param value Value
	XNOR_ENGINE void SetVec4(const std::string& keyName, const Vector4& value) const;

	/// @brief Sets a @ref Matrix (16 float, 512 bits) variable in a shader
	/// @param keyName Variable name
	/// @param value Value
	XNOR_ENGINE void SetMat4(const std::string& keyName, const Matrix& value) const;

	/// @brief Gets the internal id of the shader
	/// @return Id
	[[nodiscard]]
	XNOR_ENGINE uint32_t GetId() const;

	/// @brief Binds the shader for use
	XNOR_ENGINE void Use() const;

	/// @brief Unbinds the shader
	XNOR_ENGINE void Unuse() const;

	/// @brief Sets a specialized depth function for the shader, should only be called it before @ref CreateInRhi
	/// @param depthFunction Depth function
	void SetDepthFunction(DepthFunction depthFunction);
	
	/// @brief Sets a specialized blend function for the shader, should only be called it before @ref CreateInRhi
	/// @param blendFunction Blend function
	void SetBlendFunction(const BlendFunction& blendFunction);

private:
	uint32_t m_Id = 0;
	
	DepthFunction m_DepthFunction = DepthFunction::LESS;
	BlendFunction m_BlendFunction =
	{
		.isBlending = false,
		.sValue = BlendValue::ONE,
		.dValue = BlendValue::ZERO
	};
		

	std::array<Pointer<File>, static_cast<size_t>(ShaderType::Count)> m_Files;
	std::array<ShaderCode, static_cast<size_t>(ShaderType::Count)> m_Code;
};



END_XNOR_CORE
