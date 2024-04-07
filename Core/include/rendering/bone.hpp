#pragma once

#include "core.hpp"
#include "Maths/matrix.hpp"
#include "Maths/quaternion.hpp"
#include "Maths/vector3.hpp"

BEGIN_XNOR_CORE

class Bone
{
public:
    Bone() = default;
    ~Bone() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Bone)

    void Create(const Matrix& localMatrix, const Matrix& offsetMatrix, int32_t parent);

    Vector3 position;
    Quaternion rotation;
    Matrix local;
    Matrix global;
    Matrix globalInverse;
    int32_t parentId;
};

END_XNOR_CORE
