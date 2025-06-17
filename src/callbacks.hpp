#pragma once

class Program;

namespace Callbacks {
    void mouseCallback(GLFWwindow *window, double xposIn, double yposIn);
    void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    void framebufferSizeCallback(GLFWwindow *window, int width, int height);
}


