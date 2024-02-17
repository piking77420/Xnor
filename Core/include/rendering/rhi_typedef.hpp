#pragma once

#include "core.hpp"

BEGIN_XNOR_CORE

enum PolyGoneMode : int
{
	POINT = 0,
	LINE,
	FILL,
	
};

enum PolyGoneFace : int
{
	FRONT_LEFT = 0,
	FRONT_RIGHT,
	BACK_LEFT,
	FRONT,
	BACK,
	LEFT,
	RIGHT,
	FRONT_AND_BACK
};

enum ShaderType
{
	VERTEX,
	FRAGMENT,
	GEOMETRY,
	COMPUTE,
};

struct ShaderCode
{
	const char* shaderCode = nullptr;
	ShaderType shaderType;
};

END_XNOR_CORE