#pragma once

#include "core.hpp"
#include "scene/entity.hpp"

// Move to ToolBox
XNOR_ENGINE bool Decompose(Matrix const& modelMatrix,Vector3* translation, Quaternion* orientation,Vector3* scale ,
	 Vector3* skew, Vector4* perspective);
XNOR_ENGINE Vector3 Combine(
    Vector3 const& a,
    Vector3 const& b,
    float ascl, float bscl);

XNOR_ENGINE Vector3 ScaleVector(const Vector3& v,const float desiredLength);


BEGIN_XNOR_CORE

class SceneGraph
{
public:
    XNOR_ENGINE SceneGraph() = default;

    XNOR_ENGINE ~SceneGraph() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(SceneGraph)

    XNOR_ENGINE void Update(const List<Entity*>& entities);

    XNOR_ENGINE static Matrix GetTrs(const Matrix& trs, const Entity* parent);

    XNOR_ENGINE static void OnAttachToParent(Entity& entity);
    
private:
    XNOR_ENGINE static Matrix GetTrsOfParents(const Entity& parent);
    
    XNOR_ENGINE void UpdateTransform(Entity& entity);
};

END_XNOR_CORE
