#include "glshader.h"
#include "glbuffer.h"

void shaderLoad(shader& shader, const char* vertexShaderSource, const char* fragShaderSource) {
    // -- Vertex Shader --
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){

        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // -- Fragment Shader --
    unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragShader);

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if(!success){

        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // -- Shader linking --
    shader.id = glCreateProgram();
    glAttachShader(shader.id, vertexShader);
    glAttachShader(shader.id, fragShader);
    glLinkProgram(shader.id);

    glGetProgramiv(shader.id, GL_LINK_STATUS, &success);
    if (!success){

        glGetProgramInfoLog(shader.id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // -- cleanup --
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
}

void shaderFree(shader& shader) {
    glDeleteProgram(shader.id);
}

void shaderBind(const shader& shader) {
    glUseProgram(shader.id);
}

void shaderUnbind(void) {
    glUseProgram(0);
}

int shaderGetUniformLocation(const shader& shader, const char* name) {
    return glGetUniformLocation(shader.id, name);
}

void shaderSetTextureUnitToSampler(const shader& shader, std::string name, unsigned int value) {
    glUniform1i(shaderGetUniformLocation(shader, name.c_str()), value);
}

void shaderIntLoad(const shader& shader, int location, int value) {
    glUniform1i(location, value);
}

void shaderFloatLoad(const shader& shader, int location, float value) {
    glUniform1f(location, value);
}

void shaderVec2Load(const shader& shader, int location, vec2 value) {
    glUniform2f(location, value.x, value.y);
}

void shaderVec3Load(const shader& shader, int location, vec3 value) {
    glUniform3f(location, value.x, value.y, value.z);
}

void shaderMat3Load(const shader& shader, int location, mat3 value) {
    glUniformMatrix3fv(location, 1, GL_FALSE, value.m);
}

void shaderMat4Load(const shader& shader, int location, mat4 value) {
    glUniformMatrix4fv(location, 1, GL_FALSE, value.m);
}

void shaderSetTexture(const shader& shader, const std::vector<texture>& textures, size_t textureRef, unsigned int textureUnit, const char* sampler) {
    glActiveTexture(GL_TEXTURE0 + textureUnit);

    textureBind(textures.at(textureRef));

    glUniform1i(shaderGetUniformLocation(shader, sampler), textureUnit);
    shaderSetTextureUnitToSampler(shader, sampler, textureUnit);
}

void shaderSetTextures(const shader& shader, std::span<const texture> textures, std::span<const size_t> textureRefs) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;

    for (unsigned int i = 0; i < textureRefs.size(); ++i) {

        auto& textureReference = textureRefs[i];
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[textureReference].name;

        if      (name == "texture_diffuse")     number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")    number = std::to_string(specularNr++);
        else if (name == "texture_normal")      number = std::to_string(normalNr++);
        else if (name == "texture_height")      number = std::to_string(heightNr++);

        shaderSetTextureUnitToSampler(shader, std::string("material." + name + number).c_str(), i);
        textureBind(textures[textureReference]);
    }
}
