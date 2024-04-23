#include "resource/model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "rendering/rhi.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

Model::~Model()
{
    Rhi::DestroyModel(m_ModelId);
}

bool_t Model::Load(const uint8_t* buffer, const int64_t length)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFileFromMemory(buffer, length, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FindInvalidData | aiProcess_FixInfacingNormals | aiProcess_CalcTangentSpace);

    if (!scene)
    {
        Logger::LogError("An error occured while loading model: {}. Assimp error: {}", m_Name, importer.GetErrorString());
        return false;
    }

    if (!scene->HasMeshes())
    {
        Logger::LogError("Invalid mesh format, should contain a model: {}", m_Name);
        return false;
    }

    if (scene->mNumMeshes > 1)
    {
        Logger::LogError("Invalid mesh format, should only contain a single model: {}", m_Name);
        return false;
    }

    return Load(*scene->mMeshes[0]);
}

bool_t Model::Load(const aiMesh& loadedData)
{
    m_Vertices.resize(loadedData.mNumVertices);
    
    for (uint32_t i = 0; i < loadedData.mNumVertices; i++)
    {
        Vertex& vert = m_Vertices[i];
        vert.position = Vector3(&loadedData.mVertices[i].x);
        vert.normal = Vector3(&loadedData.mNormals[i].x);
        vert.textureCoord = Vector2(&loadedData.mTextureCoords[0][i].x);
        
        vert.tangent = Vector3(&loadedData.mBitangents[i].x);
        vert.bitangent = Vector3::Cross(vert.normal,vert.tangent);
    }

    m_Indices.resize(static_cast<size_t>(loadedData.mNumFaces) * 3);
    
    for (uint32_t i = 0; i < loadedData.mNumFaces; i++)
    {
        const aiFace& face = loadedData.mFaces[i];
        if (face.mNumIndices != 3)
        {
            Logger::LogError("Model data should be triangulated: {}", m_Name);
            return false;
        }
    
        const uint32_t baseIndex = i * 3;
        m_Indices[baseIndex + 0] = face.mIndices[0];
        m_Indices[baseIndex + 1] = face.mIndices[1];
        m_Indices[baseIndex + 2] = face.mIndices[2];
    }

    ComputeAabb(loadedData.mAABB);

    m_Loaded = true;

    return true;
}

void Model::CreateInInterface()
{
    m_ModelId = Rhi::CreateModel(m_Vertices, m_Indices);

    m_LoadedInInterface = true;
}

void Model::DestroyInInterface()
{
    Rhi::DestroyModel(m_ModelId);

    m_LoadedInInterface = false;
}

void Model::Unload()
{
    if (!m_Loaded)
        return;
    
    m_Vertices.clear();
    m_Indices.clear();

    m_Loaded = false;
}

uint32_t Model::GetId() const
{
    return m_ModelId;
}

Bound Model::GetAabb() const
{
    return m_Aabb;
}

const std::vector<Vertex>& Model::GetVertices() const
{
    return m_Vertices;
}

void Model::ComputeAabb(const aiAABB& assimpAabb)
{
    Vector3 min;
    Vector3 max;
    
    if (assimpAabb.mMax.x == 0.f && assimpAabb.mMax.y == 0.f && assimpAabb.mMax.z == 0.f &&
        assimpAabb.mMin.x == 0.f && assimpAabb.mMin.y == 0.f && assimpAabb.mMin.z == 0.f)
    {
        
        
        for (const Vertex& vertex : m_Vertices)
        {
            if (vertex.position.x < min.x)
                 min.x = vertex.position.x;
            if (vertex.position.y < min.y)
                min.y = vertex.position.y;
            if (vertex.position.z < min.z)
                min.z = vertex.position.z;

            if (vertex.position.x > max.x)
                max.x = vertex.position.x;
            if (vertex.position.y > max.y)
                max.y = vertex.position.y;
            if (vertex.position.z > max.z)
                max.z = vertex.position.z;
        }
    }
    else
    {
        min = Vector3(&assimpAabb.mMin.x);
        max = Vector3(&assimpAabb.mMax.x);
    }

   m_Aabb.SetMinMax(min, max);
}
