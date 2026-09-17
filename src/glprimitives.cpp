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

void circle::draw(const shader& s) const {
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void circle::drawWireFrame(const shader& s) const {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void sphere::draw(const shader& s) const {
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void sphere::drawWireFrame(const shader& s) const {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void triangle::draw(const shader& s) {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void triangle::drawWireFrame(const shader& s) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void cube::draw(const shader& s) {
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void cube::drawWireFrame(const shader& s) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
