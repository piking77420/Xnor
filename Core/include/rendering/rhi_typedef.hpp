#pragma once

#include "core.hpp"
#include "Maths/matrix.hpp"

BEGIN_XNOR_CORE

enum class PolyGoneMode : int32_t
{
	POINT = 0,
	LINE,
	FILL,
};

enum class PolyGoneFace : int32_t
{
	FRONT_LEFT = 1024,
	FRONT_RIGHT,
	BACK_LEFT,
	BACK_RIGHT,
	FRONT,
	BACK,
	LEFT,
	RIGHT,
	FRONT_AND_BACK 
};

enum class ShaderType : int32_t
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

enum class TextureWrapping : int32_t
{
	NONE,
	REPEAT,
	MIRRORED_REPEAT,
	CLAMP_TO_EDGE,
	CLAMP_TO_BORDER
};

enum class TextureFiltering: int32_t
{
	NONE,
	LINEAR,
	NEAREST,
};

enum class TextureType: int32_t
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

enum class TextureInternalFormat
{
	R_8,
	R_16,
	RG_8,
	RG_16,
	RGB_8,
	RGB_16,
	RGBA_8,
	RGBA_16,
	R_16F,
	RG_16F,
	RGB_16F,
	RGBA_16F,
	DEPTH_COMPONENT,
};

enum class TextureFormat
{
	RED,
	RGB,
	RGBA,
};

enum class AttachementHandle
{
	COLOR,
	POSITION,
	NORMAL,
	TEXTURECOORD,
};

struct TextureCreateInfo
{
	void* data = nullptr;
	uint32_t textureSizeWidth;
	uint32_t textureSizeHeight;
	TextureFiltering textureFiltering;
	TextureWrapping textureWrapping;
	TextureFormat textureFormat;
	TextureInternalFormat textureInternalFormat;
};

struct CameraUniformData
{
	Matrix view = Matrix::Identity();
	Matrix projection = Matrix::Identity();
	Vector3 cameraPos;
};
struct ModelUniformData
{
	Matrix model = Matrix::Identity();
	Matrix normalInvertMatrix = Matrix::Identity();
};

enum class UniformType
{
	INT,
	BOOL,
	FLOAT,
	VEC3,
	VEC4,
	MAT3,
	MAT4,
};

END_XNOR_CORE
