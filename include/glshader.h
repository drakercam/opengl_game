#ifndef GLSHADER_H
#define GLSHADER_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glmath.h"
#include "gltexture.h"
#include "glbuffer.h"

class shader {

public:
    shader(const char* vertexShaderSource, const char* fragShaderSource) {

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
        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragShader);
        glLinkProgram(id);

        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success){

            glGetProgramInfoLog(id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // -- cleanup --
        glDeleteShader(vertexShader);
        glDeleteShader(fragShader);
    }

    ~shader() {
        glDeleteProgram(this->id);
    }

    void bind() const;
    void unbind(void) const;
    int getUniformLocation(const char* name) const;
    void setTextureUnitToSampler(std::string name, unsigned int value) const;
    void intLoad(int location, int value) const;
    void floatLoad(int location, float value) const;
    void vec2Load(int location, vec2 value) const;
    void vec3Load(int location, vec3 value) const;
    void mat3Load(int location, mat3 value) const;
    void mat4Load(int location, mat4 value) const;

    void setTextures(const buffer<texture>& textures, const buffer<size_t>& textureRefs) const;

private:
    GLuint id;

};

#endif
