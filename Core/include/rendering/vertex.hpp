#pragma once

#include "core.hpp"
#include "Maths/vector2.hpp"
#include "Maths/vector3.hpp"

BEGIN_XNOR_CORE

struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 textureCoord;
	Vector3 tangent;
	Vector3 bitangent;
};

END_XNOR_CORE
