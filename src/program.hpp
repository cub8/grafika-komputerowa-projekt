#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <filesystem/filesystem.h>
#include <stb_image/stb_image.h>

#include <array>
#include <iostream>
#include <optional>
#include <vector>
#include <chrono>
#include <thread>

#include "box.hpp"
#include "callbacks.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "world_constraints.hpp"
#include "model.hpp"
#include "particle_system.hpp"
#include "contamination.hpp"
#include "wind_grid.hpp"
#include "gui.hpp"

class Program {
public:
    struct PowerPlant {
        glm::vec3 position;
        float powerMW;

        PowerPlant(const glm::vec3& pos, float mw)
            : position(pos), powerMW(mw) {}
    };

    GLFWwindow* window;
    std::optional<Shader> boxShader, planeShader, axisShader, modelShader, particleShader, windVectorShader, contaminationShader;
    std::optional<Texture> texture1, texture2, texture3, psTexture;
    std::optional<Object> box, plane, axis, vectorArrow;
    std::optional<Model> powerPlantModel;
    std::array<glm::vec3, 10> cubePositions;
    std::vector<PowerPlant> nuclearPowerPlants;
    std::vector<std::string> plantNames = {
    "Zaporozhye",
    "Forsmark",
    "Chooz",
    "Mochovce",
    "Vandellos"
    };
    std::optional<int> selectedPlantIndex;

    Camera camera;
    WindGrid windGrid;
    Contamination contaminationMask;
    ParticleSystem particleSystem;

    const unsigned int SCR_WIDTH = 1200;
    const unsigned int SCR_HEIGHT = 800;
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    bool renderWindVectors = true;

    Program(const char* programName) {
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
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            throw;
        }

        Gui::initialize(window);

        glfwSetFramebufferSizeCallback(window, Callbacks::framebufferSizeCallback);
        glfwSetScrollCallback(window, Callbacks::scrollCallback);
        glfwSetWindowUserPointer(window, this);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetMouseButtonCallback(window, Callbacks::mouseButtonCallback);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        initShaders();
        contaminationMask.initialize(SCR_WIDTH, SCR_HEIGHT);
        contaminationMask.clear();
        initTextures();
        initObjects();

        particleSystem.initialize();
        windGrid.initialize();

        selectedPlantIndex.emplace(-1);
        camera = Camera(glm::vec3(0.0f, 10.0f, 0.0f), -90.0f, -45.0f);
    }

    ~Program() {
        Gui::shutdown();
        glfwTerminate();
    }

    void renderLoop() {
        std::cout << "Render loop started\n";

        while (!glfwWindowShouldClose(window)) {
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            glfwSwapInterval(1); // vsync

            processInput(window);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            particleSystem.update(deltaTime, windGrid);

            Renderer::renderBoxes(this);
            Renderer::renderPlane(this);
            Renderer::renderAxis(this);
            Renderer::renderPlants(this);

            if (renderWindVectors)
                Renderer::renderWindVectors(this);

            Renderer::renderParticles(this);

            Gui::beginFrame();
            Gui::render(this);
            Gui::endFrame();

            glfwSwapBuffers(window);
            glfwPollEvents();
            limitFPS(60);
        }
    }

    // === Shaders ===
    Shader& getBoxShader()       { return getShader(boxShader, "Box"); }
    Shader& getPlaneShader()     { return getShader(planeShader, "Plane"); }
    Shader& getAxisShader()      { return getShader(axisShader, "Axis"); }
    Shader& getModelShader()     { return getShader(modelShader, "Model"); }
    Shader& getParticleShader()  { return getShader(particleShader, "Particle"); }
    Shader& getWindVectorShader() { return getShader(windVectorShader, "Wind Vector"); }
    Shader& getContaminationShader(){ return getShader(contaminationShader, "Contamination"); }

    // === Textures ===
    Texture& getTexture1() { return getTexture(texture1, "Texture1"); }
    Texture& getTexture2() { return getTexture(texture2, "Texture2"); }
    Texture& getTexture3() { return getTexture(texture3, "Texture3"); }
    Texture& getPsTexture(){ return getTexture(psTexture, "Particle Texture"); }

    // === Objects ===
    Object& getBox()       { return getObject(box, "Box"); }
    Object& getPlane()     { return getObject(plane, "Plane"); }
    Object& getAxis()      { return getObject(axis, "Axis"); }
    Object& getVectorArrow(){ return getObject(vectorArrow, "Vector Arrow"); }
    Model&  getPowerPlantModel() {
        if (!powerPlantModel) throw std::runtime_error("Power Plant Model not initialized");
        return *powerPlantModel;
    }

    // === Misc ===
    Camera& getCamera() { return camera; }
    WindGrid& getWindGrid() { return windGrid; }
    float getAspectRatio() const { return float(SCR_WIDTH) / float(SCR_HEIGHT); }
    float getFov() const { return camera.Zoom; }
    int getSelectedPlantIndex() const {
        if (!selectedPlantIndex) throw std::runtime_error("Selected plant index not initialized");
        return *selectedPlantIndex;
    }

private:
    void initShaders() {
        boxShader.emplace("shaders/box.vs", "shaders/box.fs");
        planeShader.emplace("shaders/plane.vs", "shaders/plane.fs");
        axisShader.emplace("shaders/axis.vs", "shaders/axis.fs");
        modelShader.emplace("shaders/model.vs", "shaders/model.fs");
        particleShader.emplace("shaders/particle.vs", "shaders/particle.fs");
        contaminationShader.emplace("shaders/contamination.vs", "shaders/contamination.fs");
        windVectorShader.emplace("shaders/wind_vector.vs", "shaders/wind_vector.fs");
    }

    void initTextures() {
        texture1.emplace("textures/container.jpg");
        texture2.emplace("textures/awesomeface.png");
        texture3.emplace("textures/europe_map.png");
        psTexture.emplace("textures/dot.png");

        getBoxShader().use();
        getBoxShader().setInt("texture1", 0);
        getBoxShader().setInt("texture2", 1);

        getPlaneShader().use();
        glActiveTexture(GL_TEXTURE0);
        texture3->bindTexture(GL_TEXTURE0);
        getPlaneShader().setInt("Tex", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, contaminationMask.getTextureID());
        getPlaneShader().setInt("ContaminationTex", 1);

        getParticleShader().use();
        getParticleShader().setInt("particleTexture", 0);

        glUseProgram(0);
    }

    void initObjects() {
        powerPlantModel.emplace("../models/cooling_tower.obj");

        nuclearPowerPlants = {
            { {22.0f, 0.0f, 4.0f},     6000.0f },
            { {3.0f, 0.0f, -10.0f},    3000.0f },
            { {-10.0f, 0.0f, 1.5f},    3120.0f },
            { {6.0f, 0.0f, 4.0f},      1950.0f },
            { {-14.0f, 0.0f, 12.0f},   1060.0f }
        };

        auto attributes = std::vector<int>{ 3, 2 };
        box.emplace(vertices, sizeof(vertices), attributes);

        const float width = WorldConstraints::ASPECT_RATIO;
        const float planeVertices[] = {
            -width, 0.0f,  1.0f,  0.0f, 1.0f,
            -width, 0.0f, -1.0f,  0.0f, 0.0f,
             width, 0.0f, -1.0f,  1.0f, 0.0f,

             width, 0.0f, -1.0f,  1.0f, 0.0f,
            -width, 0.0f,  1.0f,  0.0f, 1.0f,
             width, 0.0f,  1.0f,  1.0f, 1.0f,
        };
        plane.emplace(planeVertices, sizeof(planeVertices), attributes);

        const float axisVertices[] = {
            0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,

            0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,

            0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f
        };
        auto axisAttributes = std::vector<int>{ 3, 3 };
        axis.emplace(axisVertices, sizeof(axisVertices), axisAttributes);

        const float arrowVertices[] = {
            -0.50f, 0.0f,  0.25f,
            -0.50f, 0.0f, -0.25f,
             0.25f, 0.0f, -0.25f,

             0.25f, 0.0f, -0.25f,
            -0.50f, 0.0f,  0.25f,
             0.25f, 0.0f,  0.25f,

             0.25f, 0.0f, -0.50f,
             0.25f, 0.0f,  0.50f,
             0.75f, 0.0f,  0.00f,
        };
        auto arrowAttributes = std::vector<int>{ 3 };
        vectorArrow.emplace(arrowVertices, sizeof(arrowVertices), arrowAttributes);
    }

    void processInput(GLFWwindow* window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera.RaiseMovementSpeed();
        else
            camera.LowerMovementSpeed();

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
            contaminationMask.clear();
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
            renderWindVectors = true;
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
            renderWindVectors = false;
    }

    void limitFPS(float targetFPS) {
        static float lastTime = 0.0f;
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;

        if (deltaTime < (1.0f / targetFPS)) {
            std::this_thread::sleep_for(std::chrono::duration<float>(1.0f / targetFPS - deltaTime));
        }
        lastTime = static_cast<float>(glfwGetTime());
    }

    // Helpers
    Shader& getShader(std::optional<Shader>& shader, const std::string& name) {
        if (!shader) throw std::runtime_error(name + " Shader not initialized");
        return *shader;
    }

    Texture& getTexture(std::optional<Texture>& tex, const std::string& name) {
        if (!tex) throw std::runtime_error(name + " not initialized");
        return *tex;
    }

    Object& getObject(std::optional<Object>& obj, const std::string& name) {
        if (!obj) throw std::runtime_error(name + " not initialized");
        return *obj;
    }
};
