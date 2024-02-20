#pragma once

#include "core.hpp"
#include "Maths/matrix.hpp"

BEGIN_XNOR_CORE

static constexpr uint32_t MaxSpothLight = 10;
static constexpr uint32_t MaxPointLight = 10;
static constexpr uint32_t MaxDirectionalLight = 1;

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
	DEPTH_STENCIL
};

enum class DataType : int32_t
{
	FLOAT = 0,
	UNSIGNED_INT,
};


enum class TextureFormat
{
	RED,
	RGB,
	RGBA,
};

enum class AttachementsType
{
	COLOR,
	POSITION,
	NORMAL,
	TEXTURECOORD,
	DEPTH
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


struct PointLightData
{
	Vector3 color;
	float_t intensity;
	Vector3 position;
	float_t radius;
};

struct SpothLightData
{
	Vector3 color;
	float_t intensity;
	Vector3 position;
	float_t cutOff;
	Vector3 direction;
	float_t outerCutOff;
};

struct DirectionalData
{
	Vector3 color;
	float_t intensity;
	Vector3 direction;
};


struct GpuLightData
{
	uint32_t nbrOfPointLight;
	uint32_t nbrOfSpothLight;
	PointLightData pointLightData[MaxPointLight];
	SpothLightData spothLightData[MaxSpothLight];
	DirectionalData directionalData;
};
END_XNOR_CORE
