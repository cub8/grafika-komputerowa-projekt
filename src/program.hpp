#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <iostream>
#include <optional>
#include <vector>

#include "box.hpp"
#include "callbacks.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "texture.hpp"

class Program {
public:
    GLFWwindow *window;
    std::optional<Shader> boxShader;
    std::optional<Shader> planeShader;
    std::optional<Texture> texture1, texture2, texture3;
    std::optional<Object> box;
    std::optional<Object> plane;
    std::array<glm::vec3, 10> cubePositions;
    Camera camera;

    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    Program(const char *programName) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, programName, NULL, NULL);
        if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            throw;
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, Callbacks::framebufferSizeCallback);
        // glfwSetCursorPosCallback(window, Callbacks::mouseCallback);
        // glfwSetScrollCallback(window, Callbacks::scrollCallback);
        glfwSetWindowUserPointer(window, this);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            throw;
        }

        glEnable(GL_DEPTH_TEST);

        initShaders();
        initTextures();
        initObjects();

        camera = Camera(glm::vec3(0.0f, 5.0f, 5.0f), 0.0f, -30.0f);
    }

    ~Program() { glfwTerminate(); }

    void renderLoop() {
        while (!glfwWindowShouldClose(window)) {
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            Renderer::renderBoxes(this);
            Renderer::renderPlane(this);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    Shader &getBoxShader() {
        if (!boxShader)
            throw std::runtime_error("Box Shader not initialized");
        return *boxShader;
    }

    Shader &getPlaneShader() {
        if (!planeShader)
            throw std::runtime_error("Plane Shader not initialized");
        return *planeShader;
    }

    Texture &getTexture1() {
        if (!texture1)
            throw std::runtime_error("Texture1 not initialized");
        return *texture1;
    }

    Texture &getTexture2() {
        if (!texture2)
            throw std::runtime_error("Texture2 not initialized");
        return *texture2;
    }

    Texture &getTexture3() {
        if (!texture3)
            throw std::runtime_error("Texture2 not initialized");
        return *texture3;
    }

    Object &getObject() {
        if (!box)
            throw std::runtime_error("Object not initialized");
        return *box;
    }

    Object &getPlane() {
        if (!plane)
            throw std::runtime_error("Plane not initialized");
        return *plane;
    }

    Camera &getCamera() {
        return camera;
    }

private:
    void initShaders() {
        boxShader.emplace("shaders/box.vs", "shaders/box.fs");
        planeShader.emplace("shaders/plane.vs", "shaders/plane.fs");
    }

    void initTextures() {
        texture1.emplace("textures/container.jpg");
        texture2.emplace("textures/awesomeface.png");
        texture3.emplace("textures/europe_map.png");

        getBoxShader().use();
        getBoxShader().setInt("texture1", 0);
        getBoxShader().setInt("texture2", 1);
        
        getPlaneShader().use();
        getPlaneShader().setInt("Tex", 0);

        glUseProgram(0);
    }

    void initObjects() {
        cubePositions = {
            glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f) };

        auto attributes = std::vector<int>{ 3, 2 };
        box.emplace(vertices, sizeof(vertices), attributes);

        const float planeVertices[] = {
            // positions          // texCoords
            -1.0f, 0.0f,  1.0f,   0.0f, 1.0f,
            -1.0f, 0.0f, -1.0f,   0.0f, 0.0f,
            1.0f, 0.0f, -1.0f,   1.0f, 0.0f,

            1.0f, 0.0f, -1.0f,   1.0f, 0.0f,
            -1.0f, 0.0f,  1.0f,   0.0f, 1.0f,
            1.0f, 0.0f,  1.0f,   1.0f, 1.0f,
        };
        plane.emplace(planeVertices, sizeof(planeVertices), attributes);
    }

    void processInput(GLFWwindow *window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            camera.RaiseMovementSpeed();
        }
        else {
            camera.LowerMovementSpeed();
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }
};
