#ifndef GLSHADER_H
#define GLSHADER_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glmath.h"
#include "gltexture.h"
#include "glbuffer.h"

namespace shader {

    struct data {
        GLuint id;

        data(const char* vertexShaderSource, const char* fragShaderSource) {

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

        ~data() {
            glDeleteProgram(id);
        }
    };

    void bind(const data& s);
    void unbind(void);
    int getUniformLocation(const data& s, const char* name);
    void setTextureUnitToSampler(const data& s, std::string name, unsigned int value);
    void intLoad(int location, int value);
    void floatLoad(int location, float value);
    void vec2Load(int location, vec2 value);
    void vec3Load(int location, vec3 value);
    void mat3Load(int location, mat3 value);
    void mat4Load(int location, mat4 value);

    void setTextures(const shader::data& s, const buffer::data<texture::data>& textures, const buffer::data<GLuint>& textureRefs);
}

#endif
