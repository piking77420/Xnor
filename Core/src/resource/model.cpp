#include "resource/model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "utils/logger.hpp"

using namespace XnorCore;

void Model::Load(const uint8_t* buffer, const int64_t length)
{
    using namespace Assimp;

    Importer importer;
    const aiScene* scene = importer.ReadFileFromMemory(buffer, length, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FindInvalidData | aiProcess_FixInfacingNormals);

    if (!scene)
    {
        Logger::LogError("An error occured while loading model: %s, error: %s", m_Name.c_str(), importer.GetErrorString());
        return;
    }

    if (!scene->HasMeshes())
    {
        Logger::LogError("Invalid mesh format, should contain a model: %s", m_Name.c_str());
        return;
    }

    if (scene->mNumMeshes > 1)
    {
        Logger::LogError("Invalid mesh format, should only contain a single model: %s", m_Name.c_str());
        return;
    }

    const aiMesh* mesh = scene->mMeshes[0];

    m_Vertices.resize(mesh->mNumVertices);
    
    for (uint32_t i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex& vert = m_Vertices[i];
        vert.position = Vector3(&mesh->mVertices[i].x);
        vert.normal = Vector3(&mesh->mNormals[i].x);
        vert.textureCoord = Vector2(&mesh->mTextureCoords[0][i].x);
    }

    m_Indices.resize(static_cast<size_t>(mesh->mNumFaces) * 3);
    
    for (uint32_t i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace& face = mesh->mFaces[i];
        if (face.mNumIndices != 3)
        {
            Logger::LogError("Model data should be triangulated: %s", m_Name.c_str());
            return;
        }

        const uint32_t baseIndex = i * 3;
        m_Indices[baseIndex + 0] = face.mIndices[0];
        m_Indices[baseIndex + 1] = face.mIndices[1];
        m_Indices[baseIndex + 2] = face.mIndices[2];
    }

    m_Loaded = true;
}

void Model::Unload()
{
    if (!m_Loaded)
        return;
    
    m_Vertices.clear();
    m_Indices.clear();

    m_Loaded = false;
}
