#include "rendering/bone.hpp"

using namespace XnorCore;

void Bone::Create(const Matrix& localMatrix, const Matrix& offsetMatrix, const int32_t parent)
{
    local = localMatrix;
    global = localMatrix;
    globalInverse = offsetMatrix;
    parentId = parent;

    Vector3 scale;
    Vector3 skew;
    Vector4 perspective;
    
    local.Decompose(&position, &rotation, &scale, &skew, &perspective);
}
