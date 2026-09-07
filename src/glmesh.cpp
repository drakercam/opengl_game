#include "glmesh.h"
#include "glbuffer.h"
#include "glshader.h"

void mesh::draw(const shader& s, const buffer<texture>& textures) const {

    s.setTextures(textures, this->textureRefs);

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    texture::active(GL_TEXTURE0);       // set back to defaults once configured
}
