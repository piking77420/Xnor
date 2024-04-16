﻿#include "rendering/bone.hpp"

#include "utils/logger.hpp"

using namespace XnorCore;

void Bone::Create(const Matrix& localMatrix, const Matrix& offsetMatrix)
{
    local = localMatrix.Transposed();
    Logger::LogInfo("{}", local);
    global = local;
    globalInverse = offsetMatrix.Transposed();
    parentId = -1;
}
