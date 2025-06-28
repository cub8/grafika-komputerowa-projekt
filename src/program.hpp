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
#include "world_constraints.hpp"
#include "model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <filesystem/filesystem.h>
#include <stb_image/stb_image.h>

// to limit FPS
#include <chrono>
#include <thread>

#include "particle_system.hpp"


class Program {
public:
    GLFWwindow *window;
    std::optional<Shader> boxShader, planeShader, axisShader, modelShader, particleShader;
    std::optional<Texture> texture1, texture2, texture3, psTexture;
    std::optional<Object> box, plane, axis;
    std::optional<Model> powerPlantModel;
    std::array<glm::vec3, 10> cubePositions;
    Camera camera;

    ParticleSystem particleSystem; 

    std::vector<glm::vec3> plantPositions;

    std::optional<int> selectedPlantIndex;

    const unsigned int SCR_WIDTH = 1200;
    const unsigned int SCR_HEIGHT = 800;
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
        glfwSetScrollCallback(window, Callbacks::scrollCallback);
        glfwSetWindowUserPointer(window, this);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        glfwSetMouseButtonCallback(window, Callbacks::mouseButtonCallback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            throw;
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        initShaders();
        initTextures();
        initObjects();

        particleSystem.initialize();

        // initialize plant positions
        plantPositions = {
            {22.0f, 0.0f, 4.0f}, // Zaporoze (Ukraine)
            {3.0f, 0.0f, -10.0f}, // Forsmark (Sweden)
            {-10.0f, 0.0f, 1.5f}, // Gravelines (France)
            {6.0f, 0.0f, 4.0f}, // Mochovce (Slovakia)
            {-14.0f, 0.0f, 12.0f} // Cofrentes (Spain)
        };

        camera = Camera(glm::vec3(0.0f, 10.0f, 0.0f), -90.0f, -45.0f);
    }

    ~Program() { glfwTerminate(); }


    void limitFPS(float targetFPS) {
    static float lastTime = 0.0f;
    float currentTime = static_cast<float>(glfwGetTime());
    float deltaTime = currentTime - lastTime;

    if (deltaTime < (1.0f / targetFPS)) {
        std::this_thread::sleep_for(std::chrono::duration<float>(1.0f / targetFPS - deltaTime));
    }
    lastTime = static_cast<float>(glfwGetTime());
}


    void renderLoop() {
        std::cout << "Render loop started\n";

        while (!glfwWindowShouldClose(window)) {
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            glfwSwapInterval(1);

            processInput(window);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Renderer::renderBoxes(this);
            Renderer::renderPlane(this);
            Renderer::renderAxis(this);

            for (int i = 0; i < plantPositions.size(); ++i) {
                auto& pos = plantPositions[i];
                
                if (selectedPlantIndex && *selectedPlantIndex == i) {
                    getModelShader().use();
                    getModelShader().setVec3("overrideColor1", glm::vec3(1.f, 0.f, 0.f)); // red color if selected
                }
                else {
                    getModelShader().use();
                    getModelShader().setVec3("overrideColor1", glm::vec3(-1.f)); // no color
                }
                
                Renderer::renderModel(this, *powerPlantModel, pos, glm::vec3(0.003f));
            }

            // PARTICLES

            glm::mat4 projection = glm::perspective(
                glm::radians(camera.Zoom),
                getAspectRatio(),
                0.1f,
                1000.0f
            );
            glm::mat4 view = camera.GetViewMatrix();

            particleSystem.update(deltaTime);

            getParticleShader().use();

            // uniforms
            getParticleShader().setMat4("view", view);
            getParticleShader().setMat4("projection", projection);
            getParticleShader().setFloat("scale", 5.0f);  // scale, size of the cloud

            // bind the texture 
            glActiveTexture(GL_TEXTURE0);
            getPsTexture().bindTexture(GL_TEXTURE0);  

            // draw 
            glDepthMask(GL_FALSE);
            particleSystem.draw(view, projection);
            glDepthMask(GL_TRUE);

            glfwSwapBuffers(window);
            glfwPollEvents();

            limitFPS(60);
        }
    }

    // === SHADERS ===

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

    Shader &getAxisShader() {
        if (!axisShader)
            throw std::runtime_error("Plane Shader not initialized");
        return *axisShader;
    }

    Shader& getModelShader() {
    if (!modelShader)
        throw std::runtime_error("Model Shader not initialized");
    return *modelShader;
    }

    Shader& getParticleShader() {
    if (!particleShader)
        throw std::runtime_error("Particle Shader not initialized");
    return *particleShader;
    }


    // === TEXTURES ===


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
    
    Texture &getPsTexture() {
    if (!psTexture)
        throw std::runtime_error("Particles texture not initialized");
    return *psTexture;
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

    Object &getAxis() {
        if (!axis)
            throw std::runtime_error("Axis not initialized");
        return *axis;
    }

    Camera &getCamera() {
        return camera;
    }

    float getAspectRatio() const { return (float)SCR_WIDTH / (float)SCR_HEIGHT; }
    float getFov() const { return camera.Zoom; }




private:
    void initShaders() {
        boxShader.emplace("shaders/box.vs", "shaders/box.fs");
        planeShader.emplace("shaders/plane.vs", "shaders/plane.fs");
        axisShader.emplace("shaders/axis.vs", "shaders/axis.fs");
        modelShader.emplace("shaders/model.vs", "shaders/model.fs");
        particleShader.emplace("shaders/particle.vs", "shaders/particle.fs");
    }

    void initTextures() {
        texture1.emplace("textures/container.jpg");
        texture2.emplace("textures/awesomeface.png");
        texture3.emplace("textures/europe_map.png");
        psTexture.emplace("textures/smoke2.png");

        getBoxShader().use();
        getBoxShader().setInt("texture1", 0);
        getBoxShader().setInt("texture2", 1);

        getPlaneShader().use();
        getPlaneShader().setInt("Tex", 0);

        getParticleShader().use();
        getParticleShader().setInt("particleTexture", 0);

        glUseProgram(0);
    }



    void initObjects() {

        powerPlantModel.emplace("../models/cooling_tower.obj");


        auto attributes = std::vector<int>{ 3, 2 };
        box.emplace(vertices, sizeof(vertices), attributes);

        const float width = WorldConstraints::ASPECT_RATIO;

        const float planeVertices[] = {
            // positions                   // texCoords
            -width, 0.0f,  1.0f,          0.0f, 1.0f,
            -width, 0.0f, -1.0f,          0.0f, 0.0f,
             width, 0.0f, -1.0f,          1.0f, 0.0f,

             width, 0.0f, -1.0f,          1.0f, 0.0f,
            -width, 0.0f,  1.0f,          0.0f, 1.0f,
             width, 0.0f,  1.0f,          1.0f, 1.0f,
        };
        plane.emplace(planeVertices, sizeof(planeVertices), attributes);

        const float axisVertices[] = {
            // positions          // colors (RGB)
            0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,   // X-axis start (Red)
            1.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,   // X-axis end

            0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,   // Y-axis start (Green)
            0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   // Y-axis end

            0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,   // Z-axis start (Blue)
            0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f    // Z-axis end
        };

        auto axisAttributes = std::vector<int>{ 3, 3 };
        axis.emplace(axisVertices, sizeof(axisVertices), axisAttributes);
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
