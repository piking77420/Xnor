#pragma once

#include <map>
#include <unordered_map>
#include <vector>

#include "material.hpp"
#include "rhi_typedef.hpp"
#include "uniformBuffer.hpp"
#include "vertex.hpp"
#include "resource/model.hpp"

BEGIN_XNOR_CORE

class Rhi
{
public:

	DELETE_COPY_MOVE_OPERATIONS(Rhi)
	
	// Utils
	XNOR_ENGINE static void SetPolygonMode(PolygonFace face, PolygonMode mode);
	XNOR_ENGINE static void SetViewport(Vector2i screenSize);
	XNOR_ENGINE static void DrawQuad(uint32_t quadId);

	// Model
	XNOR_ENGINE static uint32_t CreateModel(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indicies);
	XNOR_ENGINE static bool DestroyModel(uint32_t modelId);
	XNOR_ENGINE static void DrawModel(uint32_t modelId); 
	
	// Shader
	XNOR_ENGINE static void DestroyProgram(uint32_t shaderId);
	XNOR_ENGINE static void CheckCompilationError(uint32_t shaderId, const std::string& type);
	XNOR_ENGINE static uint32_t CreateShaders(const std::vector<ShaderCode>& shaderCodes,const ShaderCreateInfo& shaderCreateInfo);
	XNOR_ENGINE static void UseShader(uint32_t shaderId);
	XNOR_ENGINE static void UnuseShader();

	XNOR_ENGINE static void SetUniform(UniformType uniformType, const void* data, uint32_t shaderId, const char* uniformKey);
	
	// TEXTURE
	XNOR_ENGINE static void CreateTexture2D(uint32_t* textureId, const TextureCreateInfo& textureCreateInfo);
	XNOR_ENGINE static void DestroyTexture(uint32_t* textureId);
	XNOR_ENGINE static void BindTexture(uint32_t unit,uint32_t textureId);
	// CubeMap
	XNOR_ENGINE static void CreateCubeMap(uint32_t* textureId, const CreateCubeMapInfo& createCubeMapInfo);


	
	// FrameBuffer
	XNOR_ENGINE static void CreateFrameBuffer(uint32_t* frameBufferId,const RenderPass& renderPass,const std::vector<const Texture*>& attechements);
	XNOR_ENGINE static void DestroyFrameBuffer(uint32_t* frameBufferId);
	XNOR_ENGINE static void BlitFrameBuffer(uint32_t readBuffer, uint32_t targetBuffer, Vector2i src0Size, Vector2i src1Size,
		Vector2i target0Size,Vector2i target1Size,Attachment attachmentTarget, TextureFiltering textureFiltering);
	XNOR_ENGINE static void BindFrameBuffer(uint32_t frameBufferId);
	XNOR_ENGINE static void UnbindFrameBuffer();

	XNOR_ENGINE static void ReadAttachement(uint32_t attachmentIndex,int32_t x, int32_t y,TextureFormat textureFormat,TextureInternalFormat textureInternalFormat,void* output);

	XNOR_ENGINE static void SwapBuffers();
	
	XNOR_ENGINE static void Initialize();

	XNOR_ENGINE static void Shutdown();

	XNOR_ENGINE static void PrepareUniform();

	XNOR_ENGINE static void SetClearColor(const Vector4& color);
	
	XNOR_ENGINE static void ClearColorAndDepth();
	
	XNOR_ENGINE static void ClearColor();

	XNOR_ENGINE static void ClearDepth();
	
	XNOR_ENGINE static void UpdateModelUniform(const ModelUniformData& modelUniformData);

	XNOR_ENGINE static void UpdateCameraUniform(const CameraUniformData& cameraUniformData);

	XNOR_ENGINE static void UpdateLight(const GpuLightData& lightData);

	XNOR_ENGINE static void BindMaterial(const Material& material);
	
	XNOR_ENGINE static void UpdateShadowMapingData(const ShadowMappingData& shadowMappingData);

	XNOR_ENGINE static TextureFormat GetFormat(const uint32_t textureFormat);

private:
	struct ModelInternal
	{
		uint32_t vao = 0;
		uint32_t vbo = 0;
		uint32_t ebo = 0;
		uint32_t nbrOfVertex = 0;
		uint32_t nbrOfIndicies = 0;
		ModelAABB aabb;
	};
	
	struct ShaderInternal
	{
		DepthFunction depthFunction;
		BlendFunction blendFunction;
		std::map<std::string, uint32_t> uniformMap;
	};

	static constexpr int32_t NullUniformLocation = -1;
	
	XNOR_ENGINE static inline std::unordered_map<uint32_t, ShaderInternal> m_ShaderMap;
	XNOR_ENGINE static inline std::unordered_map<uint32_t, ModelInternal> m_ModelMap;


	
	XNOR_ENGINE static void IsShaderValid(uint32_t shaderId);
	
	XNOR_ENGINE static int32_t GetUniformInMap(uint32_t shaderId, const char* uniformKey);

	XNOR_ENGINE static uint32_t GetOpenglDataType(DataType dataType);

	// Texture 
	XNOR_ENGINE static void AllocTexture2D(const uint32_t* textureId, const TextureCreateInfo& textureCreateInfo);
	XNOR_ENGINE static void CreateTexture(uint32_t* textureId,TextureType textureType);


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

	static void OpenglDebugCallBack(
		uint32_t source,
		uint32_t type,
		uint32_t id,
		uint32_t severity,
		size_t length,
		const char_t* message,
		const void* userParam
	);

private:
	XNOR_ENGINE static inline UniformBuffer* m_CameraUniform;
	XNOR_ENGINE static inline UniformBuffer* m_LightShadowMappingUniform;
	XNOR_ENGINE static inline UniformBuffer* m_ModelUniform;
	XNOR_ENGINE static inline UniformBuffer* m_LightUniform;
	XNOR_ENGINE static inline UniformBuffer* m_MaterialUniform;
};

END_XNOR_CORE
