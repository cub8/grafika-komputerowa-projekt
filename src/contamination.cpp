#include "Contamination.hpp"
#include <iostream>

void Contamination::initialize(unsigned int width, unsigned int height) {
    texWidth = width;
    texHeight = height;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "[ContaminationBuffer] ERROR: FBO not complete!\n";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Contamination::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, texWidth, texHeight);
}

void Contamination::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint Contamination::getTextureID() const {
    return texture;
}
