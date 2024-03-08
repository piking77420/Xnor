#pragma once

#include <map>
#include <unordered_map>
#include <vector>

#include "material.hpp"
#include "rhi_typedef.hpp"
#include "uniform_buffer.hpp"
#include "vertex.hpp"
#include "resource/model.hpp"

/// @file rhi.hpp
/// Defines the XnorCore::Rhi static class



BEGIN_XNOR_CORE

/// @brief Stands for Render Hardware Interface, provides a set of functions that interface between the application and the rendering API
class Rhi
{
	STATIC_CLASS(Rhi)
	
public:
	// Utils

	/// @brief Sets the polygon mode
	/// @param face Polygon face
	/// @param mode Polygon mode
	XNOR_ENGINE static void SetPolygonMode(PolygonFace face, PolygonMode mode);
	
	/// @brief Sets the viewport mode
	/// @param screenSize Screen size
	XNOR_ENGINE static void SetViewport(Vector2i screenSize);

	/// @brief Draws a quad
	/// @param quadId Quad id
	XNOR_ENGINE static void DrawQuad(uint32_t quadId);

	// Model

	/// @brief Creates a model
	/// @param vertices Model vertices
	/// @param indices Model indices
	/// @return Model id
	[[nodiscard]]
	XNOR_ENGINE static uint32_t CreateModel(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

	/// @brief Destroys a model
	/// @param modelId Model id
	/// @return Whether the model could be destroyed
	XNOR_ENGINE static bool_t DestroyModel(uint32_t modelId);

	/// @brief Draws a model
	/// @param modelId Model id
	XNOR_ENGINE static void DrawModel(uint32_t modelId); 
	
	// Shader

	/// @brief Destroy a shader program
	/// @param shaderId Shader id
	XNOR_ENGINE static void DestroyProgram(uint32_t shaderId);

	/// @brief Checks for compilation errors for shaders
	/// @param shaderId Shader id
	/// @param type Type
	XNOR_ENGINE static void CheckCompilationError(uint32_t shaderId, const std::string& type);

	/// @brief Creates a shader program using multiple shaders
	/// @param shaderCodes Shader codes
	/// @param shaderCreateInfo Create info
	/// @return Shader id
	[[nodiscard]]
	XNOR_ENGINE static uint32_t CreateShaders(const std::vector<ShaderCode>& shaderCodes, const ShaderCreateInfo& shaderCreateInfo);

	/// @brief Binds a shader for use
	/// @param shaderId Shader id
	XNOR_ENGINE static void UseShader(uint32_t shaderId);
	
	/// @brief Unbinds the current shader
	XNOR_ENGINE static void UnuseShader();

	/// @brief Sets a uniform variable in a shader
	/// @param uniformType Uniform type
	/// @param data Pointer to data
	/// @param shaderId Shader id
	/// @param uniformKey Uniform variable name
	XNOR_ENGINE static void SetUniform(UniformType uniformType, const void* data, uint32_t shaderId, const char_t* uniformKey);
	
	// Texture

	/// @brief Creates a 2D texture
	/// @param textureCreateInfo Texture create info
	/// @return Texture id
	XNOR_ENGINE static uint32_t CreateTexture2D(const TextureCreateInfo& textureCreateInfo);
	
	/// @brief Destroys a texture
	/// @param textureId Texture id
	XNOR_ENGINE static void DestroyTexture(uint32_t textureId);

	/// @brief Binds a texture
	/// @param unit Texture unit
	/// @param textureId Texture id
	XNOR_ENGINE static void BindTexture(uint32_t unit, uint32_t textureId);

	// CubeMap

	/// @brief Creates a cubemap 
	/// @param createCubeMapInfo Cubemap create info
	/// @return textureId Texture id
	XNOR_ENGINE static uint32_t CreateCubeMap(const CreateCubeMapInfo& createCubeMapInfo);

	// FrameBuffer

	/// @brief Create a framebuffer
	/// @param renderPass Associated @ref RenderPass
	/// @param attachments @ref Texture attachments
	/// @return Framebuffer id
	XNOR_ENGINE static uint32_t CreateFrameBuffer(const RenderPass& renderPass, const std::vector<const Texture*>& attachments);

	/// @brief Destroys a framebuffer
	/// @param frameBufferId Framebuffer id
	XNOR_ENGINE static void DestroyFrameBuffer(uint32_t frameBufferId);

	/// @brief Performs a blit framebuffer operation, which copies a region of pixels from one framebuffer to another
	/// @param readBuffer Source framebuffer id
	/// @param targetBuffer Target framebuffer id
	/// @param srcTopLeft Top left position of the source region
	/// @param srcBottomRight Bottom right position of the source region
	/// @param targetTopLeft Top left position of the target region
	/// @param targetBottomRight Top left position of the target region
	/// @param bufferFlag Target attachment
	/// @param textureFiltering Interpolation to be applied
	XNOR_ENGINE static void BlitFrameBuffer(uint32_t readBuffer, uint32_t targetBuffer, Vector2i srcTopLeft, Vector2i srcBottomRight,
		Vector2i targetTopLeft, Vector2i targetBottomRight, BufferFlag bufferFlag, TextureFiltering textureFiltering);

	/// @brief Binds a framebuffer
	/// @param frameBufferId Framebuffer id
	XNOR_ENGINE static void BindFrameBuffer(uint32_t frameBufferId);

	/// @brief Unbinds a framebuffer
	XNOR_ENGINE static void UnbindFrameBuffer();

	/// @brief Reads a single pixel of an attachment
	/// @param attachmentIndex Attachment index
	/// @param position Pixel position
	/// @param textureFormat Texture format
	/// @param textureInternalFormat Texture internal format
	/// @param output Output pointer
	XNOR_ENGINE static void GetPixelFromAttachement(uint32_t attachmentIndex, Vector2i position, TextureFormat textureFormat, TextureInternalFormat textureInternalFormat, void* output);

	/// @brief Swaps the front and back buffer
	XNOR_ENGINE static void SwapBuffers();

	/// @brief Initializes the Rhi
	XNOR_ENGINE static void Initialize();

	/// @brief Shutdowns the Rhi
	XNOR_ENGINE static void Shutdown();

	/// @brief Prepares the default uniform buffers
	XNOR_ENGINE static void PrepareUniform();

	/// @brief Sets the clear color
	/// @param color Clear color
	XNOR_ENGINE static void SetClearColor(const Vector4& color);

	/// @brief Clear the corresponding Buffers
	/// @param bufferFlag BufferFlag
	XNOR_ENGINE static void ClearBuffer(BufferFlag bufferFlag);
	
	/// @brief Updates the model @ref UniformBuffer
	/// @param modelUniformData Data
	XNOR_ENGINE static void UpdateModelUniform(const ModelUniformData& modelUniformData);

	/// @brief Updates the camera @ref UniformBuffer
	/// @param cameraUniformData Data
	XNOR_ENGINE static void UpdateCameraUniform(const CameraUniformData& cameraUniformData);

	/// @brief Updates the light @ref UniformBuffer
	/// @param lightData Data
	XNOR_ENGINE static void UpdateLight(const GpuLightData& lightData);
	
	/// @brief Updates the shadow mapping @ref UniformBuffer
	/// @param shadowMappingData Data
	XNOR_ENGINE static void UpdateShadowMappingData(const ShadowMappingData& shadowMappingData);

	/// @brief Binds a @ref Material
	/// @param material Material
	XNOR_ENGINE static void BindMaterial(const Material& material);

	/// @brief Gets the texture format based on the number of color channels
	/// @param channels Color channels
	/// @return Texture format
	XNOR_ENGINE static TextureFormat GetTextureFormatFromChannels(uint32_t channels);

private:
	struct ModelInternal
	{
		uint32_t vao = 0;
		uint32_t vbo = 0;
		uint32_t ebo = 0;
		uint32_t nbrOfVertex = 0;
		uint32_t nbrOfIndicies = 0;
		Model::Aabb aabb;
	};
	
	struct ShaderInternal
	{
		DepthFunction depthFunction{};
		BlendFunction blendFunction;
		std::map<std::string, uint32_t> uniformMap;
	};

	static constexpr int32_t NullUniformLocation = -1;
	
	XNOR_ENGINE static inline std::unordered_map<uint32_t, ShaderInternal> m_ShaderMap;
	XNOR_ENGINE static inline std::unordered_map<uint32_t, ModelInternal> m_ModelMap;

	XNOR_ENGINE static void IsShaderValid(uint32_t shaderId);
	
	XNOR_ENGINE static int32_t GetUniformInMap(uint32_t shaderId, const char_t* uniformKey);

	XNOR_ENGINE static uint32_t GetOpenglDataType(DataType dataType);

	// Texture 
	XNOR_ENGINE static void AllocTexture2D(uint32_t textureId, const TextureCreateInfo& textureCreateInfo);
	XNOR_ENGINE static uint32_t CreateTexture(TextureType textureType);

	// Enum to OpenglEnum
	XNOR_ENGINE static uint32_t GetOpengDepthEnum(DepthFunction depthFunction);
	XNOR_ENGINE static uint32_t GetOpenglShaderType(ShaderType shaderType);
	XNOR_ENGINE static std::string GetShaderTypeToString(ShaderType shaderType);
	XNOR_ENGINE static uint32_t GetOpenglTextureType(TextureType textureType);
	XNOR_ENGINE static uint32_t GetOpenglInternalFormat(TextureInternalFormat textureFormat);
	XNOR_ENGINE static uint32_t GetOpenGlTextureFormat(TextureFormat textureFormat);
	XNOR_ENGINE static uint32_t GetOpenglTextureWrapper(TextureWrapping textureWrapping);
	XNOR_ENGINE static uint32_t GetOpenglTextureFilter(TextureFiltering textureFiltering);
	XNOR_ENGINE static uint32_t GetBlendValueOpengl(BlendValue blendFunction);
	XNOR_ENGINE static uint32_t GetOpenglBufferBit(BufferFlag flag);

	XNOR_ENGINE static void OpenglDebugCallBack(
		uint32_t source,
		uint32_t type,
		uint32_t id,
		uint32_t severity,
		size_t length,
		const char_t* message,
		const void* userParam
	);

	XNOR_ENGINE static inline uint32_t m_LastFrameBufferId = 0;

	XNOR_ENGINE static inline UniformBuffer* m_CameraUniform;
	XNOR_ENGINE static inline UniformBuffer* m_LightShadowMappingUniform;
	XNOR_ENGINE static inline UniformBuffer* m_ModelUniform;
	XNOR_ENGINE static inline UniformBuffer* m_LightUniform;
	XNOR_ENGINE static inline UniformBuffer* m_MaterialUniform;
};

END_XNOR_CORE
