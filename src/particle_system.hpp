#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    float life;
    float intensity;
    float scale;
};

struct InstanceData {
    glm::vec3 pos;
    float intensity;
    float scale;
};

class ParticleSystem {
public:
    ParticleSystem();

    void initialize();
    void emit(const glm::vec3& sourcePos, int count);
    void update(float dt);
    void draw(const glm::mat4& view, const glm::mat4& proj);

private:
    std::vector<Particle> particles;
    std::vector<InstanceData> instances;

    unsigned int vao = 0;
    unsigned int vboInstance = 0;
    unsigned int quadVBO = 0;
    const size_t maxParticles = 10000;

    glm::vec3 windVelocity;

    void initGLResources();
    void updateGPUBuffer();
};
