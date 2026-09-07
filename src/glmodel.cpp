#include "glmodel.h"
#include "glbuffer.h"
#include "glmesh.h"

void model::load(const std::string path, buffer<texture>& textures) {
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene, textures);
}

void model::draw(const shader& s, const buffer<texture>& textures) {

    for (size_t i = 0; i < meshes.size(); ++i) {
        meshes.get(i).draw(s, textures);
    }
}

void model::processNode(aiNode* node, const aiScene* scene, buffer<texture>& textures) {
    for (size_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.emplace(processMesh(mesh, scene, textures));
    }

    for (size_t i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene, textures);
    }
}

mesh model::processMesh(aiMesh* m, const aiScene* scene, buffer<texture>& textures) {
    std::vector<vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<size_t> textureRefs;

    for (size_t i = 0; i < m->mNumVertices; ++i) {
        vertex vertex;

        vec3 v;
        v.x = m->mVertices[i].x;
        v.y = m->mVertices[i].y;
        v.z = m->mVertices[i].z;

        vertex.pos = v;

        if (m->HasNormals()) {
            v.x = m->mNormals[i].x;
            v.y = m->mNormals[i].y;
            v.z = m->mNormals[i].z;

            vertex.normal = v;
        }

        if (m->mTextureCoords[0]) {
            vec2 vec;

            vec.x = m->mTextureCoords[0][i].x;
            vec.y = m->mTextureCoords[0][i].y;
            vertex.uv = vec;

            v.x = m->mTangents[i].x;
            v.y = m->mTangents[i].y;
            v.z = m->mTangents[i].z;
            vertex.tangent = v;

            v.x = m->mBitangents[i].x;
            v.y = m->mBitangents[i].y;
            v.z = m->mBitangents[i].z;
            vertex.bitangent = v;
        }

        else {
            vertex.uv = {0.0f, 0.0f};
        }

        vertices.push_back(vertex);
    }

    for (size_t i = 0; i < m->mNumFaces; ++i) {
        aiFace face = m->mFaces[i];

        for (size_t j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process materials
    aiMaterial* material = scene->mMaterials[m->mMaterialIndex];

    // 1. diffuse maps
    auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", textures);
    textureRefs.insert(textureRefs.end(), diffuseMaps.begin(), diffuseMaps.end());

    // 2. specular maps
    auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", textures);
    textureRefs.insert(textureRefs.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    auto normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", textures);
    textureRefs.insert(textureRefs.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    auto heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", textures);
    textureRefs.insert(textureRefs.end(), heightMaps.begin(), heightMaps.end());

    return mesh(vertices, indices, textureRefs);
}

std::vector<size_t> model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, buffer<texture>& textures) {
    std::vector<size_t> refs;

    for (size_t i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);

        std::string filename = directory + "/" + str.C_Str();

        bool skip = false;

        for (size_t j = 0; j < textures.size(); ++j) {
            if (textures.get(j).getPath() == filename) {
                refs.push_back(j);
                skip = true;
                break;
            }
        }

        if (!skip) {
            textures.emplace(filename.c_str());

            textures.get(textures.size() - 1).setName(typeName);

            refs.push_back(textures.size() - 1);
        }
    }

    return refs;
}
