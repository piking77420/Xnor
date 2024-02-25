#pragma once

#include <array>

#include "core.hpp"
#include "Maths/math.hpp"
#include "rendering/rhi_typedef.hpp"
#include "resource/resource.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_CORE

class Shader : public Resource
{
public:
	XNOR_ENGINE static inline constexpr std::array<const char_t*, 3> VertexFileExtensions
	{
		".vert",
		".vertex",
		".vs"
	};
	
	XNOR_ENGINE static inline constexpr std::array<const char_t*, 3> FragmentFileExtensions
	{
		".frag",
		".fragment",
		".fs"
	};
	
	XNOR_ENGINE static inline constexpr std::array<const char_t*, 2> GeometryFileExtensions
	{
		".geom",
		".geometry"
	};
	
	XNOR_ENGINE static inline constexpr std::array<const char_t*, 2> ComputeFileExtensions
	{
		".comp",
		".compute"
	};

	static ShaderType FileExtensionToType(const std::string& extension);
	
	using Resource::Resource;
	
	XNOR_ENGINE void Load(const Pointer<File>& shader) override;
	
	XNOR_ENGINE void Load(const uint8_t* buffer, int64_t length) override;
	
	XNOR_ENGINE void Load(const char_t* buffer, int64_t length, ShaderType type);

	XNOR_ENGINE void CreateInRhi() override;

	XNOR_ENGINE void DestroyInRhi() override;
	
	XNOR_ENGINE void Recompile();

	XNOR_ENGINE void Unload() override;

	XNOR_ENGINE void SetInt(const std::string& keyName, int value) const;
	
	XNOR_ENGINE void SetBool(const std::string& keyName, bool value) const;
	
	XNOR_ENGINE void SetFloat(const std::string& keyName, float value) const;
	
	XNOR_ENGINE void SetVec3(const std::string& keyName, const Vector3& value) const;
	
	XNOR_ENGINE void SetVec4(const std::string& keyName, const Vector4& value) const;
	
	XNOR_ENGINE void SetMat4(const std::string& keyName, const Matrix& value) const;

	[[nodiscard]]
	uint32_t GetId() const;

	void Use() const;

	void Unuse() const;

private:
	uint32_t m_Id = 0;

	std::array<Pointer<File>, static_cast<size_t>(ShaderType::Count)> m_Files;
	std::array<ShaderCode, static_cast<size_t>(ShaderType::Count)> m_Code;
};




END_XNOR_CORE
