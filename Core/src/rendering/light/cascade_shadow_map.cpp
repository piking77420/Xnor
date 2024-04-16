#include "rendering/light/cascade_shadow_map.hpp"

#include "rendering/rhi_typedef.hpp"

using namespace XnorCore;


void CascadeShadowMap::GetCascadeCameras(std::vector<Camera>* cameras, const Camera& baseCam, Vector3 lightDir,
    Vector2i screenSize)
{
    cameras->resize(DirectionalCascadeLevel + 1);
    
    for (size_t i = 0; i < DirectionalCascadeLevel + 1; ++i)
    {
        cameras->at(i) = baseCam;
        
        if (i == 0)
        {
            GetCamera(&cameras->at(i), baseCam.near, m_CascadeLevel[i],baseCam,lightDir,screenSize);
        }
        else if (i < m_CascadeLevel.size())
        {
            GetCamera(&cameras->at(i), m_CascadeLevel[i - 1], m_CascadeLevel[i], baseCam,lightDir,screenSize);
        }
        else
        {
            GetCamera(&cameras->at(i), m_CascadeLevel[i - 1], baseCam.far,baseCam,lightDir,screenSize);

        }
    }

    
}

void CascadeShadowMap::SetCascadeLevel(const std::vector<float_t>& cascadeLevel)
{
    m_CascadeLevel = cascadeLevel;
}

void CascadeShadowMap::ComputeFrustumCorner(std::vector<Vector4>* frustumCornerWorldSpace, const Matrix& proj,
                                            const Matrix& view)
{
    Matrix invertMatrix; 
    try
    {
        invertMatrix = (proj * view).Inverted();
    }
    catch (const std::invalid_argument&)
    {
        invertMatrix = Matrix::Identity();
    }

    for (size_t x = 0; x < 2; ++x)
    {
        for (size_t y = 0; y < 2; ++y)
        {
            for (size_t z = 0; z < 2; ++z)
            {
                const Vector4 pt =
                    invertMatrix * Vector4(
                        2.0f * static_cast<float_t>(x) - 1.0f,
                        2.0f * static_cast<float_t>(y) - 1.0f,
                        2.0f * static_cast<float_t>(z) - 1.0f,
                        1.0f);
                frustumCornerWorldSpace->emplace_back(pt / pt.w);
            }
        }
    }
}

void CascadeShadowMap::GetCamera(Camera* cascadedCamera,const float_t cascadedNear, const float_t cascadedFar, const Camera& baseCamera, const Vector3 lightDir, const Vector2i screenSize)
{
    const Matrix proj = Matrix::Perspective(
            baseCamera.fov * Calc::Deg2Rad, static_cast<float_t>(screenSize.x) / static_cast<float_t>(screenSize.y), cascadedNear,
            cascadedFar);

    cascadedCamera->near = cascadedNear;
    cascadedCamera->far = cascadedFar;
    
    Matrix view;
    baseCamera.GetView(&view);

    std::vector<Vector4> corners;
    ComputeFrustumCorner(&corners,proj,view);

    Vector3 center; 
    for (const Vector4& v : corners)
    {
        center += {v.x , v.y, v.z};
    }
    center /= static_cast<float_t>(corners.size());

    cascadedCamera->position = center;
    cascadedCamera->LookAt(cascadedCamera->position + lightDir);

    const Matrix lightView = Matrix::LookAt(center + lightDir, center, Vector3::UnitY());

    float_t minX = std::numeric_limits<float_t>::max();
    float_t maxX = std::numeric_limits<float_t>::lowest();
    float_t minY = std::numeric_limits<float_t>::max();
    float_t maxY = std::numeric_limits<float_t>::lowest();
    float_t minZ = std::numeric_limits<float_t>::max();
    float_t maxZ = std::numeric_limits<float_t>::lowest();
    
    for (const Vector4& v : corners)
    {
        const auto trf = lightView * v;
        minX = std::min(minX, trf.x);
        maxX = std::max(maxX, trf.x);
        minY = std::min(minY, trf.y);
        maxY = std::max(maxY, trf.y);
        minZ = std::min(minZ, trf.z);
        maxZ = std::max(maxZ, trf.z);
    }

    // TODO Tune this parameter according to the scene
    constexpr float_t zMult = 10.0f;
    if (minZ < 0)
    {
        minZ *= zMult;
    }
    else
    {
        minZ /= zMult;
    }
    if (maxZ < 0)
    {
        maxZ /= zMult;
    }
    else
    {
        maxZ *= zMult;
    }
    
    cascadedCamera->leftRight = { minX , maxX };
    cascadedCamera->bottomtop = { minY, maxY };
}

