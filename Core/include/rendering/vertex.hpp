#pragma once

#include "core.hpp"
#include "Maths/vector2.hpp"
#include "Maths/vector3.hpp"

/// @file vertex.hpp
/// @brief Defines the XnorCore::Vertex class.

BEGIN_XNOR_CORE

/// @see <a href="https://en.wikipedia.org/wiki/Vertex_(computer_graphics)">Vertex Wikipedia page</a>
struct Vertex
{
	Vector3 position;
	Vector3 normal;
	/// @brief Texture coordinates in the range [0, 1].
	Vector2 textureCoord;
	Vector3 tangent;
	Vector3 bitangent;
};

END_XNOR_CORE
