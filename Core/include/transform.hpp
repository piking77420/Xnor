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
/// @brief Defines the XnorCore::Transform class.

BEGIN_XNOR_CORE

class SceneGraph;

/// @brief Represents a 3D transformation, containing Position, Rotation and Scaling
class XNOR_ENGINE Transform final : public Serializable
{
	REFLECTABLE_IMPL_MINIMAL(Transform);

public:
	/// @brief Returns the position of this Transform.
	const Vector3& GetPosition() const;
	
	/// @brief Sets the position of this Transform.
	Vector3& SetPosition();

	/// @brief Returns the rotation of this Transform in euler angles.
	const Vector3& GetRotationEulerAngle() const;

	/// @brief Sets the rotation of this Transform in euler angles.
	Vector3& SetRotationEulerAngle();

	/// @brief Returns the rotation of this Transform.
	const Quaternion& GetRotation() const;

	/// @brief Sets the rotation of this Transform.
	Quaternion& SetRotation();

	/// @brief Returns the scale of this Transform.
	const Vector3& GetScale() const;

	/// @brief Sets the scale of this Transform.
	Vector3& SetScale();

	/// @brief Returns whether at least one of this Transform's field was changed last frame.
	bool_t GetChanged() const;

	/// @brief World transformation matrix of the transform
	Matrix worldMatrix = Matrix::Identity();
	
private:
	/// @brief Position
	Vector3 m_Position;

	/// @brief Euler rotation
	Vector3 m_EulerRotation;

	/// @brief Scaling
	Vector3 m_Scale = Vector3(1.f);

	/// @brief Quaternion associated with the euler rotation
	Quaternion m_Rotation = Quaternion::Identity();
	
	/// @brief Whether the transform changed and needs to be updated
	bool_t m_Changed = true;

	// SceneGraph is a friend to be able to access the m_Changed private field if the transform changed between 2 frames
	friend class SceneGraph;
};

END_XNOR_CORE

/// @private
REFL_AUTO(type(XnorCore::Transform),
	field(m_Position, XnorCore::NotifyChange(&XnorCore::Transform::m_Changed)),
	field(m_EulerRotation, XnorCore::NotifyChange(&XnorCore::Transform::m_Changed)),
	field(m_Scale, XnorCore::NotifyChange(&XnorCore::Transform::m_Changed))
);

/// @brief @c std::formatter template specialization for the XnorCore::Transform type.
template <>
struct std::formatter<XnorCore::Transform>
{
    /// @brief Parses the input formatting options.
    ///
    ///	The options are specified in 2 parts, the XnorCore::Transform and the @c float options.
    /// The available XnorCore::Transform options are:
    /// - `m`: Uses line breaks for easier readability
    /// - `M`: Prints the world matrix instead of the XnorCore::Transform components
    /// - `q`: Uses a Quaternion for the rotation instead of an euler angles Vector3. Incompatible with option `M`
    /// - `d`: Uses degrees for rotation instead of radians. Incompatible with option `M` or `q`
	template<class ParseContext>
	constexpr typename ParseContext::iterator parse(ParseContext& ctx)
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

	/// @brief Formats a string using the given instance of XnorCore::Transform, according to the given options in the parse function.
	template<class FormatContext>
	typename FormatContext::iterator format(const XnorCore::Transform& transform, FormatContext& ctx) const
	{
		std::ostringstream out;

		if (m_WorldMatrix)
		{
			std::string beg = "{:";
			if (m_Multiline)
				beg += 'm';
			out << std::vformat(beg + m_Format + '}', std::make_format_args(transform.worldMatrix));
		}
		else
		{
			std::format_args args;
			if (m_QuaternionRotation)
				args = std::make_format_args(transform.GetPosition(), transform.GetRotation(), transform.GetScale());
			else
				args = std::make_format_args(transform.GetPosition(), transform.GetRotationEulerAngle() * (m_DegreeRotation ? Calc::Rad2Deg : 1.f), transform.GetScale());
        
			const char separator = m_Multiline ? '\n' : ';';

			out << std::vformat(
				"{{ {:" + m_Format + "} }} " + separator + "{{ {:" + m_Format + "} }} " + separator + " {{ {:" + m_Format + "} }}",
				args
			);
		}
        
		return std::ranges::copy(std::move(out).str(), ctx.out()).out;
	}

private:
	std::string m_Format;
	bool m_Multiline = false;
	bool m_QuaternionRotation = false;
	bool m_DegreeRotation = false;
	bool m_WorldMatrix = false;
};
