#pragma once

#include "core.hpp"
#include "Maths/matrix.hpp"

BEGIN_XNOR_CORE

static constexpr uint32_t MaxSpotLights = 10;
static constexpr uint32_t MaxPointLights = 10;
static constexpr uint32_t MaxDirectionalLights = 1;

enum class PolygonMode : int32_t
{
	Point = 0,
	Line,
	Fill,
};

enum class PolygonFace : int32_t
{
	FrontLeft = 1024,
	FrontRight,
	BackLeft,
	BackRight,
	Front,
	Back,
	Left,
	Right,
	FrontAndBack 
};

enum class ShaderType : int32_t
{
	Vertex,
	Fragment,
	Geometry,
	Compute,
};

struct ShaderCode
{
	const char* shaderCode = nullptr;
	ShaderType shaderType;
};

enum class TextureWrapping : int32_t
{
	None,
	Repeat,
	MirroredRepeat,
	ClampToEdge,
	ClampToBorder
};

enum class TextureFiltering: int32_t
{
	None,
	Linear,
	Nearest,
};

enum class TextureType: int32_t
{
	Texture1D,
	Texture2D,
	Texture3D,
	Texture1DArray,
	Texture2DArray,
	TextureRectangle,
	TextureCubeMap,
	TextureCubeMapArray,
	TextureBuffer,
	Texture2DMultisample,
	Texture2DMultisampleArray,
};

enum class TextureInternalFormat
{
	R8,
	R16,
	Rg8,
	Rg16,
	Rgb8,
	Rgb16,
	Rgba8,
	Rgba16,
	R16F,
	Rg16F,
	Rgb16F,
	Rgba16F,
	DepthComponent,
	DepthStencil
};

enum class DataType : int32_t
{
	Float = 0,
	UnsignedInt,
};

enum class TextureFormat
{
	Red,
	Rgb,
	Rgba,
};

enum class AttachementsType
{
	Color,
	Position,
	Normal,
	Texturecoord,
	Depth,
	Stencil,
	DepthAndStencil
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
	Int,
	Bool,
	Float,
	Vec3,
	Vec4,
	Mat3,
	Mat4,
};

struct PointLightData
{
	Vector3 color;
	float_t intensity;
	Vector3 position;
	float_t radius;
};

struct SpotLightData
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
	uint32_t nbrOfSpotLight;
	PointLightData pointLightData[MaxPointLights];
	SpotLightData spotLightData[MaxSpotLights];
	DirectionalData directionalData;
};

END_XNOR_CORE
