#include "glmodel.h"
#include "gltexture.h"

void modelLoad(model& model, const std::string path, std::vector<mesh>& meshes, std::vector<texture>& textures) {
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace); // aiProcess_FlipUVs |

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    model.directory = path.substr(0, path.find_last_of('/'));

    modelProcessNode(model, scene->mRootNode, scene, meshes, textures);
}

void modelDraw(const model& model, const shader& shader, std::span<const mesh> meshes, std::span<const texture> textures) {
    for (const modelMesh& modelMeshData : model.meshes) {
        shaderSetTextures(shader, textures, modelMeshData.textureRefs);
        meshDraw(meshes[modelMeshData.meshRef]);
    }
}

size_t modelMeshCount(model& model) {
    return model.meshes.size();
}

void modelProcessNode(model& model, aiNode* node, const aiScene* scene, std::vector<mesh>& meshes, std::vector<texture>& textures) {
    for (size_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        std::cout
        << "Processing mesh: "
        << node->mMeshes[i]
        << " vertices=" << mesh->mNumVertices
        << " faces=" << mesh->mNumFaces
        << '\n';

        model.meshes.push_back(modelProcessMesh(model, mesh, scene, meshes, textures));
    }

    for (size_t i = 0; i < node->mNumChildren; ++i) {
        modelProcessNode(model, node->mChildren[i], scene, meshes, textures);
    }
}

modelMesh modelProcessMesh(model& model, aiMesh* m, const aiScene* scene, std::vector<mesh>& meshes, std::vector<texture>& textures) {
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
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

            if (m->HasTangentsAndBitangents()) {
                v.x = m->mTangents[i].x;
                v.y = m->mTangents[i].y;
                v.z = m->mTangents[i].z;
                vertex.tangent = v;

                v.x = m->mBitangents[i].x;
                v.y = m->mBitangents[i].y;
                v.z = m->mBitangents[i].z;
                vertex.bitangent = v;
            }
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

    std::cout << "Material index: " << m->mMaterialIndex << '\n';

    // process materials
    aiMaterial* material = scene->mMaterials[m->mMaterialIndex];

    aiString materialName;
    material->Get(AI_MATKEY_NAME, materialName);

    std::cout << "Material name: " << materialName.C_Str() << '\n';

    // 1. diffuse maps
    std::cout << "Material texture counts:\n";
    std::cout << "  diffuse:  " << material->GetTextureCount(aiTextureType_DIFFUSE) << '\n';
    std::cout << "  specular: " << material->GetTextureCount(aiTextureType_SPECULAR) << '\n';
    std::cout << "  normal:   " << material->GetTextureCount(aiTextureType_NORMALS) << '\n';

    auto diffuseMaps = modelLoadMaterialTextures(model, material, aiTextureType_DIFFUSE, "texture_diffuse", textures);
    textureRefs.insert(textureRefs.end(), diffuseMaps.begin(), diffuseMaps.end());

    // 2. specular maps
    auto specularMaps = modelLoadMaterialTextures(model, material, aiTextureType_SPECULAR, "texture_specular", textures);
    textureRefs.insert(textureRefs.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    auto normalMaps = modelLoadMaterialTextures(model, material, aiTextureType_HEIGHT, "texture_normal", textures);
    textureRefs.insert(textureRefs.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    auto heightMaps = modelLoadMaterialTextures(model, material, aiTextureType_AMBIENT, "texture_height", textures);
    textureRefs.insert(textureRefs.end(), heightMaps.begin(), heightMaps.end());

    size_t meshRef = meshes.size();

    meshes.emplace_back();

    meshData data;
    data.vertices = std::move(vertices);
    data.indices = std::move(indices);

    meshLoad(meshes.back(), data);

    modelMesh result;
    result.meshRef = meshRef;
    result.textureRefs = std::move(textureRefs);

    return result;
}

std::vector<size_t> modelLoadMaterialTextures(model& model, aiMaterial* mat, aiTextureType type, const std::string& typeName, std::vector<texture>& textures) {
    std::vector<size_t> refs;

    for (size_t i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);

        std::string filename = model.directory + "/" + str.C_Str();
        std::cout << "Loading texture: " << filename << '\n';

        bool skip = false;

        for (size_t j = 0; j < textures.size(); ++j) {
            if (textures.at(j).path == filename) {
                refs.push_back(j);
                skip = true;
                break;
            }
        }

        if (!skip) {
            texture textureData;
            textureLoad(textureData, filename.c_str());

            textures.push_back(textureData);

            textures.back().name = typeName;

            refs.push_back(textures.size() - 1);
        }
    }

    return refs;
}
