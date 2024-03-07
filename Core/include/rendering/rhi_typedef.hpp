#pragma once

#include <Maths/matrix.hpp>

#include "core.hpp"
#include "utils/color.hpp"

/// @file rhi_typedef.hpp
/// @brief Defines some types and enumerations needed by the Rhi.

BEGIN_XNOR_CORE

class Texture;
class RenderPass;

/// @brief Maximum amount of spot lights that can exists in a same scene
static constexpr uint32_t MaxSpotLights = 100;
/// @brief Maximum amount of point lights that can exists in a same scene
static constexpr uint32_t MaxPointLights = 100;
/// @brief Maximum amount of directional lights that can exists in a same scene
static constexpr uint32_t MaxDirectionalLights = 1;

/// @brief Polygon rasterization mode
/// @see <a href="https://registry.khronos.org/OpenGL-Refpages/gl4/html/glPolygonMode.xhtml">OpenGL specification</a>
enum class PolygonMode : int32_t
{
	Point = 0,
	Line,
	Fill,
};

/// @brief Polygon draw mode
/// @see <a href="https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml">OpenGL specification</a>
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

/// @brief Polygon face
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

/// @brief Shader type
enum class ShaderType : int32_t
{
	Vertex,
	Fragment,
	Geometry,
	Compute,

	Count
};

/// @brief Encapsulates shader code information
struct ShaderCode
{
	/// @brief Pointer to raw code
	const char_t* code = nullptr;
	/// @brief Raw code length
	int32_t codeLength = 0;
	/// @brief Shader type
	ShaderType type = ShaderType::Count;
};

/// @brief %Texture wrapping, determines how a point will be sampled if it's outside of the texture
enum class TextureWrapping : int32_t
{
	None,
	/// @brief Repeats the image
	Repeat,
	/// @brief Repeats the image, also mirrors it with each repeat
	MirroredRepeat,
	/// @brief Uses the last valid border pixel of the texture
	ClampToEdge,
	/// @brief Samples a user given pixel
	ClampToBorder
};

/// @brief %Texture filtering, determines how the sampling approximation will be done
enum class TextureFiltering : int32_t
{
	None,
	/// @brief Performs linear interpolation
	Linear,
	/// @brief Selects the nearest texel
	Nearest
};

/// @brief %sTexture type
enum class TextureType : int32_t
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


/// @brief %Texture internal format
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
	DepthComponent32F,
	Depth24Stencil8,
	DepthComponent32FStencil8
};

/// @brief %Texture data type
enum class DataType : int32_t
{
	Float = 0,
	UnsignedByte,
};

/// @brief %Texture format
enum class TextureFormat
{
	Red,
	Rgb,
	Rgba,
};

/// @brief %Framebuffer %attachment
enum class Attachment
{
	/// @brief Color attachment 1
	Color01,
	/// @brief Color attachment 2
	Color02,
	/// @brief Color attachment 3
	Color03,
	/// @brief Color attachment 4
	Color04,
	/// @brief Depth attachment
	Depth,
	/// @brief Stencil attachment
	Stencil,
	/// @brief Depth and stencil attachment
	DepthAndStencil,
};

/// @brief Render target info for @ref RenderPass
struct RenderTargetInfo
{
	/// @brief Attachment
	Attachment attachment;
};

struct ClearValue
{
	Vector3 color;
	Vector2 depth;
};

/// @brief Render pass begin info
struct RenderPassBeginInfo
{
	/// @brief Render pass attached
	RenderPass* renderPass = nullptr;
	/// @brief Render area position
	Vector2 renderAreaOffset{};
	/// @brief Render area size
	Vector2 renderAreaExtent{};
	size_t clearValueSize{};
	ClearValue* clearValues = nullptr;
};

/// @brief Texture creation info
struct TextureCreateInfo
{
	/// @brief Data
	void* data = nullptr;
	/// @brief Width
	uint32_t textureSizeWidth{};
	/// @brief Height
	uint32_t textureSizeHeight{};
	/// @brief Filtering
	TextureFiltering textureFiltering{};
	/// @brief Wrapping
	TextureWrapping textureWrapping{};
	/// @brief Format
	TextureFormat textureFormat{};
	/// @brief Internal format
	TextureInternalFormat textureInternalFormat{};
	/// @brief Data type
	DataType dataType = DataType::UnsignedByte;
};

/// @brief Cube map creation info
struct CreateCubeMapInfo
{
	/// @brief Data
	const std::array<void*, 6>* datas = nullptr;
	/// @brief Width
	uint32_t textureSizeWidth{};
	/// @brief Height
	uint32_t textureSizeHeight{};
	/// @brief Filtering
	TextureFiltering textureFiltering{};
	/// @brief Wrapping
	TextureWrapping textureWrapping{};
	/// @brief Format
	TextureFormat textureFormat{};
	/// @brief Internal format
	TextureInternalFormat textureInternalFormat{};
	/// @brief Data type
	DataType dataType = DataType::UnsignedByte;
};

/// @brief Camera @ref UniformBuffer data
struct CameraUniformData
{
	/// @brief View matrix
	Matrix view = Matrix::Identity();
	/// @brief Projection matrix
	Matrix projection = Matrix::Identity();
	/// @brief Camera position
	Vector3 cameraPos;
};

/// @brief Model @ref UniformBuffer data
struct ModelUniformData
{
	/// @brief Model matrix
	Matrix model = Matrix::Identity();
	/// @brief Model matrix (inverted and transposed)
	Matrix normalInvertMatrix = Matrix::Identity();
};

/// @brief Uniform type for @ref Shader
enum class UniformType
{
	/// @brief Int
	Int,
	/// @brief Bool
	Bool,
	/// @brief Float
	Float,
	/// @brief @ref Vector3
	Vec3,
	/// @brief @ref Vector4
	Vec4,
	/// @brief [UNIMPLEMENTED]
	Mat3,
	/// @brief @ref Matrix
	Mat4,
};

/// @brief Depth function
enum class DepthFunction
{
	/// @brief Always
	Always,
	/// @brief Never
	Never,
	/// @brief <
	Less,
	/// @brief ==
	Equal,
	/// @brief <=
	LessEqual,
	/// @brief >
	Greater,
	/// @brief !=
	NotEqual,
	/// @brief >=
	GreaterEqual,
};

/// @brief Blend value
enum class BlendValue
{
	/// @brief 0
	Zero,
	/// @brief 1
	One,
	/// @brief Src
	SrcColor,
	/// @brief 1 - Src
	OneMinusSrcColor,
	/// @brief Dst
	DstColor,
	/// @brief 1 - Dst
	OneMinusDstColor,
	/// @brief Src.a
	SrcAlpha,
	/// @brief 1 - Src.a
	OneMinusSrcAlpha,
	/// @brief Dst.a
	DstAlpha,
	/// @brief 1 - Dst.a
	OneMinusDstAlpha,
	/// @brief Ct
	ConstantColor,
	/// @brief 1 - Ct
	OneMinusConstantColor,
	/// @brief Ct.a
	ConstantAlpha,
	/// @brief 1 - Ct.a
	OneMinusConstantAlpha,
};

/// @brief Blend function for @ref Shader
struct BlendFunction
{
	bool_t isBlending = false;
	BlendValue sValue;
	BlendValue dValue;
};

/// @brief Shader creation info
struct ShaderCreateInfo
{
	/// @brief Depth function
	DepthFunction depthFunction{};
	/// @brief Blend function
	BlendFunction blendFunction{};
};

/// @brief Point light @ref UniformBuffer data
struct PointLightData
{
	/// @brief Color
	Vector3 color;
	/// @brief Intensity
	float_t intensity{};
	/// @brief Position
	Vector3 position;
	/// @brief Radius
	float_t radius{};
};

/// @brief Spot light @ref UniformBuffer data
struct SpotLightData
{
	/// @brief Color
	Vector3 color;
	/// @brief Intensity
	float_t intensity{};
	/// @brief Position
	Vector3 position;
	/// @brief Cut-off
	float_t cutOff{};
	/// @brief Direction
	Vector3 direction;
	/// @brief Outer cut-off
	float_t outerCutOff{};
};

/// @brief Directional light @ref UniformBuffer data
struct DirectionalLightData
{
	/// @brief Color
	Vector3 color;
	/// @brief Intensity
	float_t intensity{};
	/// @brief Direction
	Vector3 direction;
};

/// @brief Light @ref UniformBuffer data
struct GpuLightData
{
	/// @brief Number of active point lights
	uint32_t nbrOfPointLight{};
	/// @brief Number of active spot lights
	uint32_t nbrOfSpotLight{};
	/// @brief Padding for alignment
	uint32_t padding1{};
	/// @brief Padding for alignment
	uint32_t padding2{};

	/// @brief Point light data
	PointLightData pointLightData[MaxPointLights];
	/// @brief Spot light data
	SpotLightData spotLightData[MaxSpotLights];
	/// @brief Directional light data
	DirectionalLightData directionalData[MaxDirectionalLights];
};

/// @brief Shadow mapping @ref UniformBuffer data
struct ShadowMappingData
{
	Matrix lightSpaceMatrix;
	Matrix model;
};

/// @brief Material @ref UniformBuffer data
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

enum BufferFlag : int32_t
{
	None = 0,
	ColorBit = 1 << 0,  
	DepthBit = 1 << 1,
	StencilBit = 1 << 2  
};

END_XNOR_CORE
