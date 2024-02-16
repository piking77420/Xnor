#pragma once

#include <unordered_map>
#include <vector>

#include "core.hpp"
#include "Maths/vector4.hpp"
#include "vertex.hpp"
#include "material.hpp"



BEGIN_XNOR_CORE
enum PolyGoneMode : int
{
	POINT = 0,
	LINE,
	FILL,
	
};

enum PolyGoneFace : int
{
	FRONT_LEFT = 0,
	FRONT_RIGHT ,
	BACK_LEFT ,
	FRONT,
	BACK,
	LEFT,
	RIGHT,
	FRONT_AND_BACK
};

class RHI
{
public:

	static void SetPolyGoneMode(PolyGoneFace face, PolyGoneMode mode);
	
	// Model //
	static uint32_t CreateModel(const std::vector<Vertex>& vertices,const std::vector<uint32_t>& indicies);
	static bool DestroyModel(uint32_t modelID);
	static void DrawModel(uint32_t modelID); 

	static void BindMaterial(const Material& material);

	RHI();

	~RHI();

	void SetClearColor(const Vector4& color) const;
	
	void ClearColorAndDepth() const;

private:
	
	struct ModelInternal
	{
		uint32_t vao = 0;
		uint32_t vbo = 0 ;
		uint32_t ebo = 0;
		uint32_t nbrOfVertex = 0;
		uint32_t nbrOfIndicies = 0;
	};


	static inline std::unordered_map<uint32_t,ModelInternal> m_ModelMap;
};

END_XNOR_CORE