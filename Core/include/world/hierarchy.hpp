#pragma once

#include "core.hpp"
#include "scene/entity.hpp"


XNOR_ENGINE bool decompose(Matrix const& ModelMatrix,Vector3 & Scale, Quaternion & Orientation, Vector3& Translation,
	 Vector3& Skew, Vector4& Perspective);
XNOR_ENGINE Vector3 combine(
    Vector3 const& a,
    Vector3 const& b,
    float ascl, float bscl);

XNOR_ENGINE Vector3 ScaleVector(const Vector3& v, float desiredLength);


BEGIN_XNOR_CORE
class Hierarchy
{
public:

    XNOR_ENGINE  Hierarchy() = default;

    XNOR_ENGINE  ~Hierarchy() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Hierarchy)

    XNOR_ENGINE void Update(const List<Entity*>& entities);

    XNOR_ENGINE static Matrix GetTrs(const Matrix& trs,const Entity* parent);

    XNOR_ENGINE static void OnAttachToParent(Entity& entity);
    
private:

    XNOR_ENGINE static  Matrix GetTrsOfParents(const Entity& parent);
    
    XNOR_ENGINE void UpdateTransform(Entity& entity);
};

END_XNOR_CORE