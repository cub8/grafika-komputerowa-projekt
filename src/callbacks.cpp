#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "callbacks.hpp"
#include "program.hpp"

#include <glm/gtc/matrix_inverse.hpp> 
#include <iostream>

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

    // check whether the ray intersects a nuclear plant
    bool BoundingBox::intersectsRay(const glm::vec3& origin, const glm::vec3& dir) {
        float tmin = (min.x - origin.x) / dir.x;
        float tmax = (max.x - origin.x) / dir.x;
        if (tmin > tmax) std::swap(tmin, tmax);

        float tymin = (min.y - origin.y) / dir.y;
        float tymax = (max.y - origin.y) / dir.y;
        if (tymin > tymax) std::swap(tymin, tymax);
        if ((tmin > tymax) || (tymin > tmax)) return false;

        tmin = std::max(tmin, tymin);
        tmax = std::min(tmax, tymax);

        float tzmin = (min.z - origin.z) / dir.z;
        float tzmax = (max.z - origin.z) / dir.z;
        if (tzmin > tzmax) std::swap(tzmin, tzmax);

        return !((tmin > tzmax) || (tzmin > tmax));
    }


    // raycasting
    glm::vec3 screenToWorldRay(double xpos, double ypos, Program* program) {

        float x = (2.0f * xpos) / program->SCR_WIDTH - 1.0f;  // convert X 
        float y = 1.0f - (2.0f * ypos) / program->SCR_HEIGHT; // convert Y

        // transformations
        glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);

        glm::mat4 projection = glm::perspective(glm::radians(program->getFov()), program->getAspectRatio(), 0.1f, 100.0f);
        glm::mat4 view = program->getCamera().GetViewMatrix();

        glm::vec4 rayEye = glm::inverse(projection) * rayClip;
        rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

        glm::vec3 rayWorld = glm::vec3(glm::inverse(view) * rayEye);
        return glm::normalize(rayWorld);
    }

    // check mouse click within the bounding box
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            Program* program = static_cast<Program*>(glfwGetWindowUserPointer(window));
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            glm::vec3 origin = program->getCamera().Position;
            glm::vec3 dir = screenToWorldRay(xpos, ypos, program);

            int i = 0;
            for (auto& pos : program->plantPositions) {
                BoundingBox box;
                float s = 0.5f;
                box.min = pos - glm::vec3(s, 0.0f, s);
                box.max = pos + glm::vec3(s, 1.5f, s);
                if (box.intersectsRay(origin, dir)) {

                    std::cout << "Kabooooom! at ("
                            << pos.x << ", " << pos.y << ", " << pos.z << ")\n";

                    program->particleSystem.emit(pos + glm::vec3(0, 2.5f, 0), 5000);  // number of particles
                
                    // index to color the Plant
                    program->selectedPlantIndex = i;
                    break;
                }
                ++i;
            }
        }
    }
}


