#pragma once

#include <array>
#include <Maths/vector3.hpp>

#include "camera.hpp"
#include "core.hpp"
#include "transform.hpp"
#include "resource/model.hpp"

BEGIN_XNOR_CORE
    struct Plane
{
    Vector3 normal = Vector3::UnitY();
    float_t distance = 0.f;

    constexpr Plane(Vector3 vec1, Vector3 vec2);
    
    float_t GetSignedDistanceToPlane(const Vector3& point) const;
};

class Frustum
{
    enum Face
    {
        Top,
        Bottom,
        Right,
        Left,
        Far,
        Near
    };

    DEFAULT_COPY_MOVE_OPERATIONS(Frustum)

    Frustum() = default;

    ~Frustum() = default;

    void UpdateFromCamera(const Camera& camera, float_t aspect);

    bool_t IsInFrutum(const Camera& camera, const Model::Aabb& aabb, const XnorCore::Transform& transform) const ;

private:

    bool_t AbbCollidWithPlane(const Plane& plane, const Model::Aabb& aabb, const Vector3& center) const;

    
    std::array<Plane,5> m_Plane;
};

END_XNOR_CORE