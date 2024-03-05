#pragma once

#include "core.hpp"
#include "Maths/matrix.hpp"

BEGIN_XNOR_CORE

class Texture;
class RenderPass;

static constexpr uint32_t MaxSpotLights = 100;
static constexpr uint32_t MaxPointLights = 100;
static constexpr uint32_t MaxDirectionalLights = 1;

enum class PolygonMode : int32_t
{
	Point = 0,
	Line,
	Fill,
};

enum class DrawMode : uint8_t
{
	Point = 0,
	LineStrip,
	LineLoop,
	LineStripAdjency,
	TrianglesStrip,
	TrianglesFan,
	Triangles,
	TrianglesStripAdjency,
	TrianglesAdjency,
	Patches 
	
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
	Count
};

struct ShaderCode
{
	const char_t* code = nullptr;
	int32_t codeLength = 0;
	ShaderType type = ShaderType::Count;
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
	DepthComponent16,
	DepthComponent24,
	DepthComponent32,
	DepthComponent32f,
	DepthStencil
};

enum class DataType : int32_t
{
	Float = 0,
	UnsignedByte,
};

enum class TextureFormat
{
	Red,
	Rgb,
	Rgba,
};

enum class Attachment
{
	Color01,
	Color02,
	Color03,
	Color04,
	Depth,
	Stencil,
	DepthAndStencil,
};

struct RenderTargetInfo
{
	Attachment attachment;
	bool draw;
	bool isClearing;
};

struct ClearValue
{
	Vector3 color;
	Vector2 depth;
};

struct RenderPassBeginInfo
{
	RenderPass* renderPass = nullptr;
	Vector2 renderAreaOffset;
	Vector2 renderAreaExtent;
	size_t clearValueSize;
	ClearValue* clearValues = nullptr;
};

struct TextureCreateInfo
{
	void* data;
	uint32_t textureSizeWidth;
	uint32_t textureSizeHeight;
	TextureFiltering textureFiltering;
	TextureWrapping textureWrapping;
	TextureFormat textureFormat;
	TextureInternalFormat textureInternalFormat;
	DataType dataType = DataType::UnsignedByte;
};

struct CreateCubeMapInfo
{
	const std::array<void*,6>* datas = nullptr;
	uint32_t textureSizeWidth;
	uint32_t textureSizeHeight;
	TextureFiltering textureFiltering;
	TextureWrapping textureWrapping;
	TextureFormat textureFormat;
	TextureInternalFormat textureInternalFormat;
	DataType dataType = DataType::UnsignedByte;
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

enum class DepthFunction
{
	ALWAYS,
	NEVER,
	LESS,
	EQUAL,
	LEAQUAL,
	GREATER,
	NOTEQUAL,
	GEQUAL
};

enum class BlendValue
{
	ZERO,
	ONE,
	SRC_COLOR,
	ONE_MINUS_SRC_COLOR,
	DST_COLOR,
	ONE_MINUS_DST_COLOR,
	SRC_ALPHA,
	ONE_MINUS_SRC_ALPHA,
	DST_ALPHA,
	ONE_MINUS_DST_ALPHA,
	CONSTANT_COLOR,
	ONE_MINUS_CONSTANT_COLOR,
	CONSTANT_ALPHA,
	ONE_MINUS_CONSTANT_ALPHA,
};

struct BlendFunction
{
	bool isBlending = false;
	BlendValue sValue;
	BlendValue dValue;
};

struct ShaderCreateInfo
{
	DepthFunction depthFunction;
	BlendFunction blendFunction;
};

struct PointLightData
{
	Vector3 color;
	float_t intensity{};
	Vector3 position;
	float_t radius{};
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
	float_t intensity{};
	Vector3 direction;
};

struct GpuLightData
{
	uint32_t nbrOfPointLight;
	uint32_t nbrOfSpotLight;
	uint32_t padding1;
	uint32_t padding2;
	PointLightData pointLightData[MaxPointLights];
	SpotLightData spotLightData[MaxSpotLights];

	DirectionalData directionalData;
};

struct ShadowMappingData
{
	Matrix lightSpaceMatrix;
	Matrix model;
};

// modifies in shader
struct MaterialData
{
	int32_t hasAlbedoMap = 0;
	int32_t hasNormalmap = 0;
};

enum Gbuffer : int32_t
{
	GbufferPosition = 4,
	GbufferNormal,
	GbufferAlbedo
};


END_XNOR_CORE
