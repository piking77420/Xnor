#include "resource/model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "rendering/rhi.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

Model::~Model()
{
    RHI::DestroyModel(m_ModelId);
}

void Model::Load(const uint8_t* buffer, const int64_t length)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFileFromMemory(buffer, length, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FindInvalidData | aiProcess_FixInfacingNormals);

    if (!scene)
    {
        Logger::LogError("An error occured while loading model: {}. Assimp error: {}", m_Name, importer.GetErrorString());
        return;
    }

    if (!scene->HasMeshes())
    {
        Logger::LogError("Invalid mesh format, should contain a model: {}", m_Name);
        return;
    }

    if (scene->mNumMeshes > 1)
    {
        Logger::LogError("Invalid mesh format, should only contain a single model: {}", m_Name);
        return;
    }

    Load(*scene->mMeshes[0]);
}

void Model::Load(const aiMesh& loadedData)
{
    m_Vertices.resize(loadedData.mNumVertices);
    
    for (uint32_t i = 0; i < loadedData.mNumVertices; i++)
    {
        Vertex& vert = m_Vertices[i];
        vert.position = Vector3(&loadedData.mVertices[i].x);
        vert.normal = Vector3(&loadedData.mNormals[i].x);
        vert.textureCoord = Vector2(&loadedData.mTextureCoords[0][i].x);
    }

    m_Indices.resize(static_cast<size_t>(loadedData.mNumFaces) * 3);
    
    for (uint32_t i = 0; i < loadedData.mNumFaces; i++)
    {
        const aiFace& face = loadedData.mFaces[i];
        if (face.mNumIndices != 3)
        {
            Logger::LogError("Model data should be triangulated: {}", m_Name);
            return;
        }
    
        const uint32_t baseIndex = i * 3;
        m_Indices[baseIndex + 0] = face.mIndices[0];
        m_Indices[baseIndex + 1] = face.mIndices[1];
        m_Indices[baseIndex + 2] = face.mIndices[2];
    }

    m_Loaded = true;

    m_ModelId = RHI::CreateModel(m_Vertices, m_Indices);
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
    return  m_ModelId;
}
