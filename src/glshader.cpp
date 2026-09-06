#include "glshader.h"
#include "glbuffer.h"

void shader::bind(const data& s) {
    glUseProgram(s.id);
}

void shader::unbind(void) {
    glUseProgram(0);
}

int shader::getUniformLocation(const shader::data& s, const char* name) {
    return glGetUniformLocation(s.id, name);
}

void shader::setTextureUnitToSampler(const data& s, std::string name, unsigned int value) {
    glUniform1i(getUniformLocation(s, name.c_str()), value);
}

void shader::intLoad(int location, int value) {
    glUniform1i(location, value);
}

void shader::floatLoad(int location, float value) {
    glUniform1f(location, value);
}

void shader::vec2Load(int location, vec2 value) {
    glUniform2f(location, value.x, value.y);
}

void shader::vec3Load(int location, vec3 value) {
    glUniform3f(location, value.x, value.y, value.z);
}

void shader::mat3Load(int location, mat3 value) {
    glUniformMatrix3fv(location, 1, GL_FALSE, value.m);
}

void shader::mat4Load(int location, mat4 value) {
    glUniformMatrix4fv(location, 1, GL_FALSE, value.m);
}

void shader::setTextures(const shader::data& s, const buffer::data<texture::data>& textures, const buffer::data<GLuint>& textureRefs) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    for (unsigned int i = 0; i < buffer::size(textureRefs); ++i) {

        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures.elements[textureRefs.elements.at(i)].name;

        if      (name == "texture_diffuse")     number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")    number = std::to_string(specularNr++);
        else if (name == "texture_normal")      number = std::to_string(normalNr++);
        else if (name == "texture_height")      number = std::to_string(heightNr++);

        shader::setTextureUnitToSampler(s, std::string("material." + name + number).c_str(), i);
        texture::bind(textures.elements.at(i));
    }
}
