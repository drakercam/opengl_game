#include "glmesh.h"
#include "glbuffer.h"
#include "glshader.h"

void mesh::draw(const mesh::data& mesh, const shader::data& s, const buffer::data<texture::data>& textures) {

    shader::setTextures(s, textures, mesh.textureRefs);

    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, buffer::size(mesh.indices), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    texture::active(GL_TEXTURE0);       // set back to defaults once configured
}
