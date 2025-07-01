#pragma once

#include <glad/glad.h>
#include <vector>
#include "particle_system.hpp"

class Contamination {
public:
    void initialize(unsigned int width, unsigned int height);
    void bind();
    void unbind();
    void clear();
    GLuint getTextureID() const;

private:
    GLuint texture = 0;
    GLuint fbo = 0;

    unsigned int texWidth = 0;
    unsigned int texHeight = 0;
};
