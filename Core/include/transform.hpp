#pragma once

#include <format>
#include <sstream>

#include <Maths/matrix.hpp>
#include <Maths/quaternion.hpp>
#include <Maths/vector3.hpp>

#include "core.hpp"
#include "serialization/serializer.hpp"
#include "utils/serializable.hpp"

/// @file transform.hpp

BEGIN_XNOR_CORE

/// @brief Represents a 3D transformation, containing Position, Rotation and Scaling
class XNOR_ENGINE Transform final : public Serializable
{
	REFLECTABLE_IMPL_MINIMAL(Transform)

public:
	/// @brief Gets the world position of the transform
	/// @return World position
	[[nodiscard]]
	Vector3 GetWorldPos() const;

	/// @brief Position
	Vector3 position;

	/// @brief Euler rotation
	Vector3 eulerRotation;

	/// @brief Scaling
	Vector3 scale = Vector3(1.f);

	/// @brief Quaternion associated with the euler rotation
	Quaternion rotation = Quaternion::Identity();

	/// @brief World transformation matrix of the transform
	Matrix worldMatrix = Matrix::Identity();
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Transform),
	field(position),
	field(eulerRotation),
	field(scale)
)

template <>
struct std::formatter<XnorCore::Transform>
{
	template<class ParseContext>
	constexpr typename ParseContext::iterator parse(ParseContext& ctx);

	template<class FormatContext>
	typename FormatContext::iterator format(const XnorCore::Transform& transform, FormatContext& ctx) const;

private:
	std::string m_Format;
	bool m_Multiline = false;
	bool m_QuaternionRotation = false;
	bool m_DegreeRotation = false;
	bool m_WorldMatrix = false;
};

template <class ParseContext>
constexpr typename ParseContext::iterator std::formatter<XnorCore::Transform>::parse(ParseContext& ctx)
{
	auto it = ctx.begin();
	if (it == ctx.end())
		return it;

	if (*it == 'm')
	{
		m_Multiline = true;
		++it;
	}

	if (*it == 'M')
	{
		m_WorldMatrix = true;
		++it;
	}

	if (*it == 'q')
	{
		if (m_WorldMatrix)
			throw std::format_error("Invalid format args for XnorCore::Transform: cannot use option 'q' with option 'M'");
            
		m_QuaternionRotation = true;
		++it;
	}

	if (*it == 'd')
	{
		if (m_WorldMatrix || m_QuaternionRotation)
			throw std::format_error("Invalid format args for XnorCore::Transform: cannot use option 'd' with option 'M' or 'q'");
            
		m_DegreeRotation = true;
		++it;
	}

	while (*it != '}' && it != ctx.end())
		m_Format += *it++;

	return it;
}

template <class FormatContext>
typename FormatContext::iterator std::formatter<XnorCore::Transform>::format(
	const XnorCore::Transform& transform,
	FormatContext& ctx
) const
{
	std::ostringstream out;

	if (m_WorldMatrix)
	{
		out << std::vformat("{:" + m_Format + '}', std::make_format_args(transform.worldMatrix));
	}
	else
	{
		std::format_args args;
		if (m_QuaternionRotation)
			args = std::make_format_args(transform.position, transform.rotation, transform.scale);
		else
			args = std::make_format_args(transform.position, transform.eulerRotation * (m_DegreeRotation ? Calc::Rad2Deg : 1.f), transform.scale);
        
		const char separator = m_Multiline ? '\n' : ';';

		out << std::vformat(
			"{{ {:" + m_Format + "} }} " + separator + "{{ {:" + m_Format + "} }} " + separator + " {{ {:" + m_Format + "} }}",
			args
		);
	}
        
	return std::ranges::copy(std::move(out).str(), ctx.out()).out;
}
