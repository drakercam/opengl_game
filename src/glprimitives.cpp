#include "glprimitives.h"
#include "glbuffer.h"
#include "glshader.h"
#include "gltexture.h"

void rectangle::draw(const shader& s) const {
    //s.setTextures(textures, this->textureRefs);

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    //texture::active(GL_TEXTURE0);
}

void rectangle::drawWireFrame(const shader& s) const {
    //s.setTextures(textures, this->textureRefs);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //texture::active(GL_TEXTURE0);
}

void triangle::draw(const shader& s, const buffer<texture>& textures) {
    s.setTextures(textures, this->textureRefs);

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    texture::active(GL_TEXTURE0);
}

void triangle::drawWireFrame(const shader& s, const buffer<texture>& textures) {
    s.setTextures(textures, this->textureRefs);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    texture::active(GL_TEXTURE0);
}

void cube::draw(const shader& s, const buffer<texture>& textures) {
    s.setTextures(textures, this->textureRefs);

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    texture::active(GL_TEXTURE0);
}

void cube::drawWireFrame(const shader& s, const buffer<texture>& textures) {
    s.setTextures(textures, this->textureRefs);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    texture::active(GL_TEXTURE0);
}
