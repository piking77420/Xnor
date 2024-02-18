#pragma once

#include <unordered_map>
#include <vector>

#include "material.hpp"
#include "rhi_typedef.hpp"
#include "uniformBuffer.hpp"
#include "Maths/vector4.hpp"
#include "vertex.hpp"


BEGIN_XNOR_CORE


class RHI
{
public:

	// Utils
	XNOR_ENGINE static void SetPolyGoneMode(PolyGoneFace face, PolyGoneMode mode);
	
	// Model //
	XNOR_ENGINE static uint32_t CreateModel(const std::vector<Vertex>& vertices,const std::vector<uint32_t>& indicies);
	XNOR_ENGINE static bool DestroyModel(uint32_t modelID);
	XNOR_ENGINE static void DrawModel(uint32_t modelID); 

	// Material
	XNOR_ENGINE static void BindMaterial(const Material& material);

	// Shader
	XNOR_ENGINE static void DestroyShader(uint32_t id);
	XNOR_ENGINE static void CheckCompilationError(uint32_t shaderId,const std::string& type);
	XNOR_ENGINE static  uint32_t CreateShader(const std::vector<ShaderCode>& shaderCodes);
	XNOR_ENGINE static  void UseShader(const uint32_t shaderID);

	// TEXTURE
	XNOR_ENGINE  static void CreateTexture(uint32_t* textureId,TextureCreateInfo textureCreateInfo);
	XNOR_ENGINE  static void DestroyTexture(const uint32_t* textureId);


	XNOR_ENGINE  static void BindTexture(TextureType textureType,uint32_t textureID);
	XNOR_ENGINE  static void UnBindTexture(TextureType textureType);

	// FrammeBuffer 
	XNOR_ENGINE static void CreateFrameBuffer(uint32_t* frameBufferID);
	XNOR_ENGINE static void DestroyFrameBuffer(uint32_t* frameBufferID);

	
	
private:

	static uint32_t GetOpenglShaderType(ShaderType shaderType);
	
	static std::string GetShaderTypeToString(ShaderType shaderType);
	
	static void ComputeTextureWrapper(uint32_t textureID, TextureWrapping textureWrapping);
	
	static void ComputeOpenglTextureFilter(uint32_t textureID, TextureFiltering textureFilter);

	static uint32_t TextureTypeToOpenglTexture(TextureType textureType);

	static uint32_t GetOpenglFormatFromTextureFormat(TextureFormat textureFormat);


public:
	
	XNOR_ENGINE RHI();

	XNOR_ENGINE ~RHI();

	XNOR_ENGINE void PrepareUniform();

	XNOR_ENGINE void SetClearColor(const Vector4& color) const;
	
	XNOR_ENGINE void ClearColorAndDepth() const;

	XNOR_ENGINE void UpdateModelUniform(const ModelUniformData& modelUniformData) const;

	XNOR_ENGINE void UpdateCameraUniform(const CameraUniformData& cameraUniformData) const;

private:
	
	struct ModelInternal
	{
		uint32_t vao = 0;
		uint32_t vbo = 0 ;
		uint32_t ebo = 0;
		uint32_t nbrOfVertex = 0;
		uint32_t nbrOfIndicies = 0;
	};

	mutable UniformBuffer* m_CameraUniform;
	mutable UniformBuffer* m_ModelUniform;


	XNOR_ENGINE static inline std::unordered_map<uint32_t,ModelInternal> m_ModelMap;
};

END_XNOR_CORE