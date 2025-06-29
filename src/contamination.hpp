#pragma once

#include <glad/glad.h>

class Contamination {
public:
    void initialize(unsigned int width, unsigned int height);
    void bind();
    void unbind();
    void clear();
    GLuint getTextureID() const;

private:
    GLuint fbo = 0;
    GLuint texture = 0;
    unsigned int texWidth = 0;
    unsigned int texHeight = 0;
};
