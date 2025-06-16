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
#include "camera.hpp"
#include "object.hpp"
#include "shader.hpp"
#include "texture.hpp"

class Program {
public:
  GLFWwindow *window;
  std::optional<Shader> shader;
  std::optional<Texture> texture1, texture2;
  std::optional<Object> object;
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
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
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

    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
  }

  ~Program() { glfwTerminate(); }

  void renderLoop() {
    while (!glfwWindowShouldClose(window)) {
      float currentFrame = static_cast<float>(glfwGetTime());
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      processInput(window);

      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      getTexture1().bindTexture(GL_TEXTURE0);
      getTexture2().bindTexture(GL_TEXTURE1);

      getShader().use();

      glm::mat4 projection =
          glm::perspective(glm::radians(camera.Zoom),
                           (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
      getShader().setMat4("projection", projection);

      glm::mat4 view = camera.GetViewMatrix();
      getShader().setMat4("view", view);
      getShader().setMat4("projection", projection);

      getObject().bindVertexArray();
      for (unsigned int i = 0; i < 10; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle),
                            glm::vec3(1.0f, 0.3f, 0.5f));
        getShader().setMat4("model", model);
        getObject().draw();
      }

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  Shader &getShader() {
    if (!shader)
      throw std::runtime_error("Shader not initialized");
    return *shader;
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

  Object &getObject() {
    if (!object)
      throw std::runtime_error("Object not initialized");
    return *object;
  }

private:
  void initShaders() {
    shader.emplace("shaders/coordinate_systems.vs",
                   "shaders/coordinate_systems.fs");
  }

  void initTextures() {
    texture1.emplace("textures/container.jpg");
    texture2.emplace("textures/awesomeface.png");

    getShader().use();
    getShader().setInt("texture1", 0);
    getShader().setInt("texture2", 1);
  }

  void initObjects() {
    cubePositions = {
        glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

    auto attributes = std::vector<int>{3, 2};
    object.emplace(vertices, sizeof(vertices), attributes);
  }

  void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
      camera.RaiseMovementSpeed();
    } else {
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

  static void mouseCallback(GLFWwindow *window, double xposIn, double yposIn) {
    Program *program;

    void *ptr = glfwGetWindowUserPointer(window);
    if (ptr) {
      program = static_cast<Program *>(ptr);
    } else {
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

  static void scrollCallback(GLFWwindow *window, double xoffset,
                             double yoffset) {
    Program *program;

    void *ptr = glfwGetWindowUserPointer(window);
    if (ptr) {
      program = static_cast<Program *>(ptr);
    } else {
      throw std::runtime_error("Scroll: can't find window pointer");
    }

    program->camera.ProcessMouseScroll(static_cast<float>(yoffset));
  }

  static void framebufferSizeCallback(GLFWwindow *window, int width,
                                      int height) {
    glViewport(0, 0, width, height);
  }
};
