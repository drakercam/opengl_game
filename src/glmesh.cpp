#include "glmesh.h"
#include "glbuffer.h"
#include "glshader.h"

void mesh::draw(const shader& s) const {
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void mesh::drawWireFrame(const shader& s) const {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
