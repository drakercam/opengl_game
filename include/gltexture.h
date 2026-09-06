#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <iostream>
#include <stb_image.h>
#include <glad/glad.h>

namespace texture {

    struct data {
        GLuint id;
        std::string name;
        std::string path;

        data(const char* imagePath) {

            // std::cout << "texture: start\n";
            this->path = imagePath;

            glGenTextures(1, &this->id);

            // std::cout << "texture: generated " << this->id << '\n';

            glBindTexture(GL_TEXTURE_2D, this->id);

            // std::cout << "texture: bound\n";

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            // std::cout << "texture: parameters 1\n";

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // std::cout << "texture: parameters 2\n";

            int width, height, numColourChannels;

            stbi_set_flip_vertically_on_load(true);

            unsigned char* data =
            stbi_load(imagePath, &width, &height, &numColourChannels, 4);

            // std::cout << "texture: stbi_load returned\n";
            // std::cout << "width: " << width << '\n';
            // std::cout << "height: " << height << '\n';
            // std::cout << "channels: " << numColourChannels << '\n';
            // std::cout << "data: " << static_cast<void*>(data) << '\n';

            if (data) {
                // std::cout << "texture: uploading\n";

                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RGBA,
                    width,
                    height,
                    0,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    data
                );

                // std::cout << "texture: uploaded\n";

                glGenerateMipmap(GL_TEXTURE_2D);

                // std::cout << "texture: mipmaps generated\n";
            }
            else {
                // std::cout << "failed to load::ERROR::TEXTURE" << std::endl;
            }

            stbi_image_free(data);
        }
    };

    void active(int textureUnit);
    void bind(const data& t);
    void unbind(void);
}

#endif
