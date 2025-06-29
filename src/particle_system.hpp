#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>  // for glm::linearRand
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "wind_grid.hpp"

struct Particle {
    glm::vec3 position;
    glm::vec3 direction;
    float velocity;
    float life;
    float intensity;
    float scale;
};

struct InstanceData {
    glm::vec3 pos;
    float intensity;
    float scale;
};

const float windVelocityScale = 0.10f;

class ParticleSystem {
public:
    ParticleSystem();

    void initialize();
    void emit(const glm::vec3& sourcePos, int count);
    void update(float deltaTime, WindGrid& windGrid);
    void adjustToWind(Particle& particle, WindGrid& windGrid);
    float calculateWindInfluence(Particle& particle, const WindVector& windVector);
    void draw();

private:
    std::vector<Particle> particles;
    std::vector<InstanceData> instances;

    unsigned int vao = 0;
    unsigned int vboInstance = 0;
    unsigned int quadVBO = 0;
    const size_t maxParticles = 10000;

    glm::vec3 windDirection;
    float windVelocity;

    void initGLResources();
    void updateGPUBuffer();
};
