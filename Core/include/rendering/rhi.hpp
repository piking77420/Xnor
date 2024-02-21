#pragma once

#include <map>
#include <unordered_map>
#include <vector>

#include "material.hpp"
#include "rhi_typedef.hpp"
#include "uniformBuffer.hpp"
#include "vertex.hpp"

BEGIN_XNOR_CORE

class RHI
{
public:
	// Utils
	XNOR_ENGINE static void SetPolygonMode(PolygonFace face, PolygonMode mode);
	XNOR_ENGINE static void SetViewport(Vector2i screenSize);
	
	// Model
	XNOR_ENGINE static uint32_t CreateModel(const std::vector<Vertex>& vertices,const std::vector<uint32_t>& indicies);
	XNOR_ENGINE static bool DestroyModel(uint32_t modelId);
	XNOR_ENGINE static void DrawModel(uint32_t modelId); 

	// Material
	XNOR_ENGINE static void BindMaterial(const Material& material);

	// Shader
	XNOR_ENGINE static void DestroyShader(uint32_t shaderId);
	XNOR_ENGINE static void CheckCompilationError(uint32_t shaderId,const std::string& type);
	XNOR_ENGINE static uint32_t CreateShader(const std::vector<ShaderCode>& shaderCodes);
	XNOR_ENGINE static void UseShader(uint32_t shaderId);
	XNOR_ENGINE static void UnuseShader();

	XNOR_ENGINE static void SetUniform(UniformType uniformType, const void* data, uint32_t shaderId, const char* uniformKey);
	
	// TEXTURE
	XNOR_ENGINE  static void CreateTexture(uint32_t* textureId, const TextureCreateInfo& textureCreateInfo);
	XNOR_ENGINE  static void DestroyTexture(const uint32_t* textureId);

	XNOR_ENGINE  static void BindTexture(uint32_t unit,uint32_t textureId);
	XNOR_ENGINE  static void UnbindTexture(TextureType textureType);

	// FrameBuffer
	XNOR_ENGINE static void CreateFrameBuffer(uint32_t* frameBufferId,uint32_t renderPassId,const std::vector<Texture*>& outTargets);
	XNOR_ENGINE static void DestroyFrameBuffer(const uint32_t* frameBufferId);
	
	XNOR_ENGINE static void BindFrameBuffer(uint32_t frameBufferId);
	XNOR_ENGINE static void UnbindFrameBuffer();

	// RenderPass 
	XNOR_ENGINE static void CreateRenderPass(uint32_t* renderPassId,const std::vector<AttachementsType>& attachementsType);

	XNOR_ENGINE static void CreateColorAttachement(Texture* texture, vec2i size);

private:
	struct ModelInternal
	{
		uint32_t vao = 0;
		uint32_t vbo = 0;
		uint32_t ebo = 0;
		uint32_t nbrOfVertex = 0;
		uint32_t nbrOfIndicies = 0;
	};

	struct ShaderInternal
	{
		uint32_t id;
		std::map<std::string, uint32_t> uniformMap;
	};

	struct RenderPassIternal
	{
		std::vector<AttachementsType> attachementsType;
	};

	static constexpr int32_t NullUniformLocation = -1;
	
	XNOR_ENGINE static inline std::unordered_map<uint32_t, ShaderInternal> m_ShaderMap;
	XNOR_ENGINE static inline std::unordered_map<uint32_t, ModelInternal> m_ModelMap;
	XNOR_ENGINE static inline std::unordered_map<uint32_t, RenderPassIternal> m_RenderPassMap;

	static uint32_t GetOpenglShaderType(ShaderType shaderType);
	
	static std::string GetShaderTypeToString(ShaderType shaderType);
	
	static void ComputeTextureWrapper(uint32_t textureId, TextureWrapping textureWrapping);
	
	static void ComputeOpenglTextureFilter(uint32_t textureId, TextureFiltering textureFilter);

	static uint32_t TextureTypeToOpenglTexture(TextureType textureType);

	static uint32_t GetOpenglInternalFormat(TextureInternalFormat textureFormat);

	static uint32_t GetOpenGlTextureFormat(TextureFormat textureFormat);
	
	static void IsShaderValid(uint32_t shaderId);
	
	static int GetUniformInMap(uint32_t shaderId, const char* uniformKey);

public:
	XNOR_ENGINE RHI();

	XNOR_ENGINE ~RHI();

	XNOR_ENGINE void PrepareUniform();

	XNOR_ENGINE void SetClearColor(const Vector4& color) const;
	
	XNOR_ENGINE void ClearColorAndDepth() const;

	XNOR_ENGINE void UpdateModelUniform(const ModelUniformData& modelUniformData) const;

	XNOR_ENGINE void UpdateCameraUniform(const CameraUniformData& cameraUniformData) const;

	XNOR_ENGINE void UpdateLight(const GpuLightData& lightData) const;

private:
	 mutable UniformBuffer* m_CameraUniform;
	 mutable UniformBuffer* m_ModelUniform;
	 mutable UniformBuffer* m_LightUniform;
};

END_XNOR_CORE
