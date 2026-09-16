#include "glshader.h"
#include "glbuffer.h"

void shader::bind() const {
    glUseProgram(this->id);
}

void shader::unbind(void) const {
    glUseProgram(0);
}

int shader::getUniformLocation(const char* name) const {
    return glGetUniformLocation(this->id, name);
}

void shader::setTextureUnitToSampler(std::string name, unsigned int value) const {
    glUniform1i(getUniformLocation(name.c_str()), value);
}

void shader::intLoad(int location, int value) const {
    glUniform1i(location, value);
}

void shader::floatLoad(int location, float value) const {
    glUniform1f(location, value);
}

void shader::vec2Load(int location, vec2 value) const {
    glUniform2f(location, value.x, value.y);
}

void shader::vec3Load(int location, vec3 value) const {
    glUniform3f(location, value.x, value.y, value.z);
}

void shader::mat3Load(int location, mat3 value) const {
    glUniformMatrix3fv(location, 1, GL_FALSE, value.m);
}

void shader::mat4Load(int location, mat4 value) const {
    glUniformMatrix4fv(location, 1, GL_FALSE, value.m);
}

void shader::setTextures(const buffer<texture>& textures, const buffer<size_t>& textureRefs) const {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;

    for (unsigned int i = 0; i < textureRefs.size(); ++i) {

        auto& textureReference = textureRefs.get(i);
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures.get(textureReference).getName();

        if      (name == "texture_diffuse")     number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")    number = std::to_string(specularNr++);
        else if (name == "texture_normal")      number = std::to_string(normalNr++);
        else if (name == "texture_height")      number = std::to_string(heightNr++);

        shader::setTextureUnitToSampler(std::string("material." + name + number).c_str(), i);
        textures.get(textureReference).bind();
    }
}
