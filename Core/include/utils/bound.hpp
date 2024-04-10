#pragma once

#include "core.hpp"
#include "transform.hpp"
#include "Maths/vector3.hpp"
#include "rendering/plane.hpp"

BEGIN_XNOR_CORE
    /// @brief Struct used to store the minimum and maximum bounds of the AABB of a Model.
class XNOR_ENGINE Bound
{
public:
    static Bound GetAabbFromTransform(const Bound& bound,const Transform& transform);
    
     /// @brief The extents of the Bounding Box. This is always size of the Bounds.
    Vector3 extents;
    /// @brief The center bound of this Bound.
    Vector3 center = Vector3::Zero();

    DEFAULT_COPY_MOVE_OPERATIONS(Bound)
    
    Bound(Vector3 newCenter, Vector3 newSize);
    
    Bound() = default;

    ~Bound() = default;

    bool_t operator==(const Bound& other) const;

    Vector3 GetMin() const;

    Vector3 GetMax() const;

    Vector3 GetSize() const;

    void SetMinMax(Vector3 newmin, Vector3 newMax);

    
    bool_t Intersect(const Bound& otherBound) const;

    bool_t Countain(const Bound& otherBound) const;

    bool_t IsOnPlane(const Plane& frustum) const;

    void Encapsulate(const Bound& plane);

    void Encapsulate(Vector3 point);


private:

    static Bound ReturnAabbFromMatrix(const Bound& bound, const Matrix& matrix, const Vector3 center);
};

END_XNOR_CORE