#include "gltexture.h"

void texture::active(int textureUnit) {
    glActiveTexture(textureUnit);
}

void texture::bind(const texture::data& t) {

    glBindTexture(GL_TEXTURE_2D, t.id);
}

void texture::unbind(void) {

    glBindTexture(GL_TEXTURE_2D, 0);
}
