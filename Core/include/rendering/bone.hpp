#pragma once

#include "core.hpp"
#include "Maths/matrix.hpp"
#include "Maths/quaternion.hpp"
#include "Maths/vector3.hpp"
#include "utils/list.hpp"

BEGIN_XNOR_CORE

class Bone
{
public:
    Bone() = default;
    ~Bone() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Bone)

    void Create(const Matrix& localMatrix, const Matrix& offsetMatrix);

    std::string name = "Bone_Unnamed";
    Vector3 position;
    Quaternion rotation;
    Matrix local;
    Matrix global;
    Matrix globalInverse;
    int32_t parentId;
    List<int32_t> children;
};

END_XNOR_CORE
