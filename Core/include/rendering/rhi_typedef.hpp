#pragma once

#include <Maths/matrix.hpp>
#include <Maths/vector2i.hpp>

#include "core.hpp"
#include "utils/color.hpp"

/// @file rhi_typedef.hpp
/// @brief Defines various types and enumerations needed by XnorCore::Rhi.

BEGIN_XNOR_CORE
class FrameBuffer;
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
enum class DrawMode
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
	R32F,
	R32Uint,
	Srgb,
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
	UnsignedByte64,
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
	/// @brief Color attachment 0
	Color00,
	/// @brief Color attachment 1
	Color01,
	/// @brief Color attachment 2
	Color02,
	/// @brief Color attachment 3
	Color03,
	/// @brief Color attachment 4
	Color04,
	/// @brief Color attachment 5
	Color05,
	/// @brief Color attachment 6
	Color06,
	/// @brief Color attachment 7
	Color07,
	/// @brief Color attachment 8
	Color08,
	/// @brief Color attachment 9
	Color09,
	/// @brief Color attachment 10
	Color10,
	/// @brief Color attachment 11
	Color11,
	/// @brief Color attachment 12
	Color12,
	/// @brief Color attachment 13
	Color13,
	/// @brief Color attachment 14
	Color14,
	/// @brief Color attachment 15
	Color15,
	/// @brief Color attachment 16
	Color16,
	/// @brief Color attachment 17
	Color17,
	/// @brief Color attachment 18
	Color18,
	/// @brief Color attachment 19
	Color19,
	/// @brief Color attachment 20
	Color20,
	/// @brief Depth attachment
	Depth,
	/// @brief Stencil attachment
	Stencil,
	/// @brief Depth and stencil attachment
	DepthAndStencil,
};

/// @brief Render target info for RenderPass
struct RenderTargetInfo
{
	/// @brief Attachment
	Attachment attachment;

	bool isDrawingOn = true;
};


/// @brief Texture creation info
struct TextureCreateInfo
{
	/// @brief Data
	void* data = nullptr;
	/// @brief Size
	Vector2i size = Vector2i::Zero();
	/// @brief Filtering
	TextureFiltering filtering{};
	/// @brief Wrapping
	TextureWrapping wrapping{};
	/// @brief Format
	TextureFormat format{};
	/// @brief Internal format
	TextureInternalFormat internalFormat{};
	/// @brief Data type
	DataType dataType = DataType::UnsignedByte;
};

/// @brief Cube map creation info
struct CreateCubeMapInfo
{
	/// @brief Data
	const std::array<void*, 6>* datas = nullptr;
	/// @brief Size
	Vector2i size = Vector2i::Zero();
	/// @brief Filtering
	TextureFiltering filtering{};
	/// @brief Wrapping
	TextureWrapping wrapping{};
	/// @brief Format
	TextureFormat format{};
	/// @brief Internal format
	TextureInternalFormat internalFormat{};
	/// @brief Data type
	DataType dataType = DataType::UnsignedByte;
};

/// @brief Camera UniformBuffer data
struct CameraUniformData
{
	/// @brief View matrix
	Matrix view = Matrix::Identity();
	/// @brief Projection matrix
	Matrix projection = Matrix::Identity();
	/// @brief Camera position
	Vector3 cameraPos;
};

/// @brief Model UniformBuffer data
struct ModelUniformData
{
	/// @brief Model matrix
	Matrix model = Matrix::Identity();
	/// @brief Model matrix (inverted and transposed)
	Matrix normalInvertMatrix = Matrix::Identity();

	uint64_t meshRenderIndex = 0;
};

/// @brief Uniform type for Shader
enum class UniformType
{
	/// @brief Int
	Int,
	/// @brief Bool
	Bool,
	/// @brief Float
	Float,
	/// @brief Vector3
	Vec3,
	/// @brief Vector4
	Vec4,
	/// @brief Matrix3
	Mat3,
	/// @brief Matrix
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

/// @brief Blend function for Shader
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

/// @brief Point light UniformBuffer data
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

/// @brief Spot light UniformBuffer data
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

/// @brief Directional light UniformBuffer data
struct DirectionalLightData
{
	/// @brief Color
	Vector3 color;
	/// @brief Intensity
	float_t intensity{};
	/// @brief Direction
	Vector3 direction;
};

/// @brief Light UniformBuffer data
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

/// @brief Shadow mapping UniformBuffer data
struct ShadowMappingData
{
	Matrix lightSpaceMatrix;
	Matrix model;
};

/// @brief Material UniformBuffer data
struct MaterialData
{
	Vector3 albedoColor;
	int32_t hasAlbedoMap;
	int32_t hasMetallicMap;
	int32_t hasRoughnessMap;
	int32_t hasNormalMap;
	int32_t hasAmbiantOcclusionMap;

	float_t metallic = 0.f;
	float_t roughness = 0.f;
	float_t reflectance = 0.f;
	float_t emissive = 0.f;
	float_t ambiantOccusion = 0.f;
};

/// @brief The type of GBuffer.
enum Gbuffer : int32_t
{
	GbufferPosition = 4,
	GbufferNormal,
	GbufferAlbedo,
	GmetallicRoughessReflectance,
	GemissiveAmbiantOcclusion,
};


enum MaterialTextureEnum : int32_t
{
	Albedo = 0,
	Metallic,
	Roughness,
	Normal,
	AmbiantOcclusion,
};

/// @brief Buffer attachment flags.
enum BufferFlag 
{
	BufferFlagNone = 0,
	BufferFlagColorBit = 1 << 0,  
	BufferFlagDepthBit = 1 << 1,
	BufferFlagStencilBit = 1 << 2  
};


/// @brief Render pass begin info
struct RenderPassBeginInfo
{
	/// @brief Render pass attached
	FrameBuffer* frameBuffer = nullptr;
	/// @brief Render area position
	Vector2i renderAreaOffset{};
	/// @brief Render area size
	Vector2i renderAreaExtent{};

	BufferFlag clearBufferFlags;

	Vector4 clearColor;
};


END_XNOR_CORE
