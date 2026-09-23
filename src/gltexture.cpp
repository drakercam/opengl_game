#include "gltexture.h"
#include <GL/gl.h>

void textureLoad(texture& texture, const char* imagePath) {
    glGenTextures(1, &texture.id);

    // std::cout << "texture: generated " << this->id << '\n';

    glBindTexture(GL_TEXTURE_2D, texture.id);

    // std::cout << "texture: bound\n";

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // std::cout << "texture: parameters 1\n";

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // std::cout << "texture: parameters 2\n";

    texture.path = imagePath;

    int width, height, numColourChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data =
    stbi_load(texture.path.c_str(), &width, &height, &numColourChannels, 4);

    std::cout
    << "Texture: " << texture.path
    << " width=" << width
    << " height=" << height
    << " channels=" << numColourChannels
    << '\n';

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
        std::cout << "Failed to load texture: "
        << texture.path << '\n';

        std::cout << "Reason: "
        << stbi_failure_reason()
        << '\n';

        return;
    }

    stbi_image_free(data);
}

void textureLoadGlyph(texture& texture, const unsigned char* data, int width, int height) {
    glGenTextures(1, &texture.id);

    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        width,
        height,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        data
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_EDGE
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_NEAREST
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_NEAREST
    );

    glBindTexture(GL_TEXTURE_2D, 0);
}

void textureActive(int textureUnit) {
    glActiveTexture(textureUnit);
}

void textureBind(const texture& texture) {
    glBindTexture(GL_TEXTURE_2D, texture.id);
}

void textureUnbind(void) {
    glBindTexture(GL_TEXTURE_2D, 0);
}
