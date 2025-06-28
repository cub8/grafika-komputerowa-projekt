#pragma once

#include <glad/glad.h>

class Contamination {
public:
    void initialize(unsigned int width = 1024, unsigned int height = 1024);
    void bind();
    void unbind();
    GLuint getTextureID() const;

private:
    GLuint fbo = 0;
    GLuint texture = 0;
    unsigned int texWidth = 0;
    unsigned int texHeight = 0;
};
