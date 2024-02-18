#pragma once

#include "core.hpp"
#include "Maths/matrix.hpp"

BEGIN_XNOR_CORE
	enum PolyGoneMode : int
{
	POINT = 0,
	LINE,
	FILL,
};

enum PolyGoneFace : int
{
	FRONT_LEFT = 1024,
	FRONT_RIGHT,
	BACK_LEFT ,
	BACK_RIGHT ,
	FRONT ,
	BACK ,
	LEFT ,
	RIGHT ,
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

enum class TextureFormat
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
	RED,
	RGB,
	RGBA,
};

enum class AttachementHandle
{
	Color,
	Position,
	Normal,
	TextureCoord,
};


struct TextureCreateInfo
{
	void* data;
	uint32_t textureSizeWidth;
	uint32_t textureSizeHeight;
	TextureFiltering textureFiltering;
	TextureWrapping textureWrapping;
	TextureFormat textureFormat;
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
	Float,
	Vec3,
	Vec4,
	mat3,
	mat4,
};

END_XNOR_CORE