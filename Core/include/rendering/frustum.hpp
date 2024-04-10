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

    Plane(Vector3 vec1, Vector3 vec2);

    Plane() = default;

    ~Plane();

    DEFAULT_COPY_MOVE_OPERATIONS(Plane)
    
    float_t GetSignedDistanceToPlane(const Vector3& point) const;
};

class Frustum
{
public:
    enum Face
    {
        Top,
        Bottom,
        Right,
        Left,
        Far,
        Near
    };

    std::array<Plane,5> plane;


    DEFAULT_COPY_MOVE_OPERATIONS(Frustum)

    Frustum() = default;

    ~Frustum() = default;

    void UpdateFromCamera(const Camera& camera, float_t aspect);

private:
};

END_XNOR_CORE