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

enum ShaderType : int
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

enum class TextureWrapping: int
{
	REPEAT,
	MIRRORED_REPEAT,
	CLAMP_TO_EDGE,
	CLAMP_TO_BORDER
};


enum class TextureFiltering: int
{
	LINEAR,
	NEAREST,
};

enum class TextureType: int
{
	TEXTURE_1D,
	TEXTURE_2D,
	TEXTURE_3D,
	TEXTURE_1D_ARRAY,
	TEXTURE_2D_ARRAY,
	TEXTURE_RECTANGLE,
	TEXTURE_CUBE_MAP,
	TEXTURE_CUBE_MAP_ARRAY,
	TEXTURE_BUFFER,
	TEXTURE_2D_MULTISAMPLE,
	TEXTURE_2D_MULTISAMPLE_ARRAY,
};

END_XNOR_CORE