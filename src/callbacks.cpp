#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "callbacks.hpp"
#include "program.hpp"

namespace Callbacks {
    void mouseCallback(GLFWwindow *window, double xposIn, double yposIn) {
        Program *program;

        void *ptr = glfwGetWindowUserPointer(window);
        if (ptr) {
            program = static_cast<Program *>(ptr);
        }
        else {
            throw std::runtime_error("Mouse: can't find window pointer");
        }

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (program->firstMouse) {
            program->lastX = xpos;
            program->lastY = ypos;
            program->firstMouse = false;
        }

        float xoffset = xpos - program->lastX;
        float yoffset = program->lastY - ypos;

        program->lastX = xpos;
        program->lastY = ypos;

        program->camera.ProcessMouseMovement(xoffset, yoffset);
    }

    void scrollCallback(GLFWwindow *window, double xoffset,
        double yoffset) {
        Program *program;

        void *ptr = glfwGetWindowUserPointer(window);
        if (ptr) {
            program = static_cast<Program *>(ptr);
        }
        else {
            throw std::runtime_error("Scroll: can't find window pointer");
        }

        program->camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }

    void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }
}


