#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <iostream>
#include <stb_image.h>
#include <glad/glad.h>

class texture {
public:
    texture(const char* imagePath) {
        this->path = imagePath;
        load();
    }

    static void active(int textureUnit);
    void bind(void) const;
    void unbind(void) const;

    const std::string getName() const {
        return name;
    }

    const std::string getPath() const {
        return path;
    }

    void setName(const std::string& name) {
        this->name = name;
    }

private:
    GLuint id;
    std::string name;
    std::string path;

    void load();
};

#endif
