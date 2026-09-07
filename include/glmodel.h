#ifndef GLMODEL_H
#define GLMODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "glshader.h"
#include "glbuffer.h"
#include "glmesh.h"

class model {

public:
    model(const std::string path, buffer<texture>& textures) {
        load(path, textures);
    }

    void draw(const shader& s, const buffer<texture>& textures);

private:
    buffer<mesh> meshes;
    std::string directory;

    void load(const std::string path, buffer<texture>& textures);
    void processNode(aiNode* node, const aiScene* scene, buffer<texture>& textures);
    mesh processMesh(aiMesh* mesh, const aiScene* scene, buffer<texture>& textures);
    std::vector<size_t> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, buffer<texture>& textures);
};

#endif
