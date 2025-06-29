#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Program;

namespace Callbacks {
    void mouseCallback(GLFWwindow *window, double xposIn, double yposIn);
    void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    void framebufferSizeCallback(GLFWwindow *window, int width, int height);

    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    
    glm::vec3 screenToWorldRay(double xpos, double ypos, Program* program);

    struct BoundingBox {
        glm::vec3 min;
        glm::vec3 max;
        bool intersectsRay(const glm::vec3& origin, const glm::vec3& dir);
    };

}


