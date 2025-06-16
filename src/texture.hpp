#pragma once

#include <filesystem/filesystem.h>
#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <iostream>

class Texture {
public:
  GLuint textureID;

  Texture(const std::string &texturePath) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(FileSystem::getPath(texturePath).c_str(),
                                    &width, &height, &nrChannels, 0);

    if (data) {
      const int format = getFormat(nrChannels);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
  }

  void bindTexture(GLenum texture) { 
    glActiveTexture(texture); 
    glBindTexture(GL_TEXTURE_2D, textureID);
  }

private:
  int getFormat(const int nrChannels) {
    int value;
    switch (nrChannels) {
    case 3:
      value = GL_RGB;
      break;
    case 4:
      value = GL_RGBA;
      break;
    default:
      std::cout << "Unknown number of channels" << std::endl;
      value = 0;
    }

    return value;
  }
};
