#include "glprimitives.h"
#include "glbuffer.h"
#include "glshader.h"
#include "gltexture.h"

void rectangle::draw(const rectangle::data& rect, const shader::data& s, const buffer::data<texture::data>& textures) {
    shader::setTextures(s, textures, rect.textureRefs);

    glBindVertexArray(rect.VAO);
    glDrawElements(GL_TRIANGLES, buffer::size(rect.indices), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    texture::active(GL_TEXTURE0);
}

void rectangle::drawWireFrame(const rectangle::data& rect, const shader::data& s, const buffer::data<texture::data>& textures) {
    shader::setTextures(s, textures, rect.textureRefs);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(rect.VAO);
    glDrawElements(GL_TRIANGLES, buffer::size(rect.indices), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    texture::active(GL_TEXTURE0);
}

void triangle::draw(const triangle::data& triangle, const shader::data& s, const buffer::data<texture::data>& textures) {
    shader::setTextures(s, textures, triangle.textureRefs);

    glBindVertexArray(triangle.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    texture::active(GL_TEXTURE0);
}

void triangle::drawWireFrame(const triangle::data& triangle, const shader::data& s, const buffer::data<texture::data>& textures) {
    shader::setTextures(s, textures, triangle.textureRefs);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(triangle.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    texture::active(GL_TEXTURE0);
}

void cube::draw(const cube::data& cube, const shader::data& s, const buffer::data<texture::data>& textures) {
    shader::setTextures(s, textures, cube.textureRefs);

    glBindVertexArray(cube.VAO);
    glDrawElements(GL_TRIANGLES, buffer::size(cube.indices), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    texture::active(GL_TEXTURE0);
}

void cube::drawWireFrame(const cube::data& cube, const shader::data& s, const buffer::data<texture::data>& textures) {
    shader::setTextures(s, textures, cube.textureRefs);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(cube.VAO);
    glDrawElements(GL_TRIANGLES, buffer::size(cube.indices), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    texture::active(GL_TEXTURE0);
}
