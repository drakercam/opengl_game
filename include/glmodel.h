#ifndef GLMODEL_H
#define GLMODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cstddef>
#include <span>
#include <string>
#include <utility>
#include <vector>
#include "glmesh.h"
#include "glshader.h"
#include "gltexture.h"

struct modelMesh {
    size_t meshRef = 0;
    std::vector<size_t> textureRefs;
};

struct model {
    std::vector<modelMesh> meshes;
    std::string directory;
};

void modelLoad(model& model, const std::string path, std::vector<mesh>& meshes, std::vector<texture>& textures);
void modelDraw(const model& model, const shader& shader, std::span<const mesh> meshes, std::span<const texture> textures);
size_t modelMeshCount(model& model);
void modelProcessNode(model& model, aiNode* node, const aiScene* scene, std::vector<mesh>& meshes, std::vector<texture>& textures);
modelMesh modelProcessMesh(model& model, aiMesh* m, const aiScene* scene, std::vector<mesh>& meshes, std::vector<texture>& textures);
std::vector<size_t> modelLoadMaterialTextures(model& model, aiMaterial* mat, aiTextureType type, const std::string& typeName, std::vector<texture>& textures);

#endif
