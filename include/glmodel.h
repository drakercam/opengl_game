#ifndef GLMODEL_H
#define GLMODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "glshader.h"
#include "glbuffer.h"
#include "glmesh.h"

namespace model {

    static void processNode(buffer::data<mesh::data>& meshes, aiNode* node, const aiScene* scene);
    static mesh::data processMesh(aiMesh* mesh, const aiScene* scene);

    struct data {

        buffer::data<mesh::data> meshes;
        std::string directory;

        data(const std::string path) {

            Assimp::Importer import;
            const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
                return;
            }

            directory = path.substr(0, path.find_last_of('/'));

            model::processNode(meshes, scene->mRootNode, scene);
        }
    };

    static void draw(model::data& model, const mesh::data& mesh, const shader::data& s, const buffer::data<texture::data>& textures);

}

#endif
