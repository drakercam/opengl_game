#include "glmodel.h"
#include "glbuffer.h"

void model::draw(model::data& model, const mesh::data& mesh, const shader::data& s, const buffer::data<texture::data>& textures) {

    for (size_t i = 0; i < buffer::size(model.meshes); ++i) {
        mesh::draw(buffer::get(model.meshes, i), s, textures);
    }
}

void model::processNode(buffer::data<mesh::data>& meshes, aiNode* node, const aiScene* scene) {
    for (size_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        buffer::emplace(meshes, model::processMesh(mesh, scene));
    }

    for (size_t i = 0; i < node->mNumChildren; ++i) {
        model::processNode(meshes, node->mChildren[i], scene);
    }
}

mesh::data model::processMesh(aiMesh* mesh, const aiScene* scene) {

}
