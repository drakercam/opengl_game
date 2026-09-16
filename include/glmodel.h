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
    model(const std::string path, buffer<mesh>& meshes, buffer<texture>& textures) {
        load(path, meshes, textures);
        std::cout << "Model meshes: " << meshRefs.size() << '\n';

        for (size_t i = 0; i < meshRefs.size(); ++i) {
            std::cout
            << "  meshRef[" << i << "] = "
            << meshRefs.get(i)
            << '\n';
        }
    }

    void draw(const shader& s, const buffer<mesh>& meshes, const buffer<texture>& textures);

    size_t meshCount() const {
        return meshRefs.size();
    }

private:
    buffer<size_t> meshRefs;
    std::string directory;

    void load(const std::string path, buffer<mesh>& meshes, buffer<texture>& textures);
    void processNode(aiNode* node, const aiScene* scene, buffer<mesh>& meshes, buffer<texture>& textures);
    void processMesh(aiMesh* m, const aiScene* scene, buffer<mesh>& meshes, buffer<texture>& textures);
    std::vector<size_t> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, buffer<texture>& textures);
};

#endif
