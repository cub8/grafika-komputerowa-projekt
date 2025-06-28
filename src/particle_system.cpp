#include "particle_system.hpp"
#include <glm/gtc/random.hpp>  // for glm::linearRand
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

ParticleSystem::ParticleSystem()
{
    windVelocity = glm::normalize(glm::vec3(-2.f, 0.0f, 1.5f)) * 2.f;
}

void ParticleSystem::initialize() {
    initGLResources();
}

void ParticleSystem::initGLResources()
{
    // quad vertices (x, y)
    float quadVerts[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f,  0.5f
    };

    //VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //Quad VBO
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glGenBuffers(1, &vboInstance);
    glBindBuffer(GL_ARRAY_BUFFER, vboInstance);

    // allocate empty buffer initially
    glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceData) * maxParticles, NULL, GL_STREAM_DRAW);

    // pos 
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, pos));
    glVertexAttribDivisor(1, 1);
    // intensity (float)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, intensity));
    glVertexAttribDivisor(2, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void ParticleSystem::emit(const glm::vec3& sourcePos, int count)
{
    for(int i = 0; i < count; ++i) {
        Particle p;
        p.position  = sourcePos;
        // random jitter
        p.velocity  = windVelocity + glm::vec3(
            glm::linearRand(-0.1f, 0.1f),
            glm::linearRand(-0.05f, 0.05f),
            glm::linearRand(-0.1f, 0.1f)
        );
        p.life      = glm::linearRand(2.0f, 5.0f);
        p.intensity = 1.0f;
        particles.push_back(p);

        if (particles.size() >= maxParticles) break;
    }
}

void ParticleSystem::update(float dt)
{
    for(auto it = particles.begin(); it != particles.end(); ) {
        it->position += it->velocity * dt;
        it->life     -= dt;
        it->intensity = glm::clamp(it->life / 5.0f, 0.0f, 1.0f);
        if(it->life <= 0.0f) {
            it = particles.erase(it);
        } else {
            ++it;
        }
    }
}

void ParticleSystem::updateGPUBuffer()
{
    instances.clear();
    instances.reserve(particles.size());
    for(const auto& p : particles) {
        instances.push_back({ p.position, p.intensity });
    }
    glBindBuffer(GL_ARRAY_BUFFER, vboInstance);
    glBufferSubData(GL_ARRAY_BUFFER, 0, instances.size() * sizeof(InstanceData), instances.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::draw(const glm::mat4& view, const glm::mat4& proj)
{
    updateGPUBuffer();

    glBindVertexArray(vao);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (GLsizei)instances.size());
    glBindVertexArray(0);
}
