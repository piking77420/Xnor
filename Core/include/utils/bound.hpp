#pragma once

#include "core.hpp"
#include "transform.hpp"
#include "Maths/vector3.hpp"

BEGIN_XNOR_CORE
    /// @brief Struct used to store the minimum and maximum bounds of the AABB of a Model.
class XNOR_ENGINE Bound
{
public:
    /// @brief The minimum bound of this AABB.
    Vector3 min { std::numeric_limits<float_t>::max() };
    /// @brief The maximum bound of this AABB.
    Vector3 max { std::numeric_limits<float_t>::min() };
     /// @brief The extents of the Bounding Box. This is always size of the Bounds.
    Vector3 size;
    /// @brief The center bound of this Bound.
    Vector3 center = Vector3::Zero();
    
    Bound(Vector3 newCenter, Vector3 newSize);
    
    Bound() = default;

    ~Bound() = default;

    void SetMinMax(Vector3 newmin, Vector3 newMax);

    static Bound GetAabbFromTransform(const Bound& bound,const Transform& transform);

    bool_t Intersect(const Bound& otherBound);

    void Encapsulate(const Bound& encaspulateBound);
    
private:
    
};

END_XNOR_CORE