#include "particle_system.hpp"


void ParticleSystem::initialize() {
    initGLResources();
}

void ParticleSystem::initGLResources() {
    float quadVerts[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f,  0.5f
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);

    glGenBuffers(1, &vboInstance);
    glBindBuffer(GL_ARRAY_BUFFER, vboInstance);

    glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceData) * maxParticles, NULL, GL_STREAM_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void *)offsetof(InstanceData, pos));
    glVertexAttribDivisor(1, 1);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void *)offsetof(InstanceData, intensity));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void *)offsetof(InstanceData, scale));
    glVertexAttribDivisor(3, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


std::tuple<float, float, float, float, int> ParticleSystem::computeParams(float powerMW) const {
    float t = glm::clamp(powerMW / 10000.0f, 0.0f, 1.0f);
    float minLife = 1.0f + t * 2.0f;
    float maxLife = 2.0f + t * 5.0f;
    float minSize = 0.1f + t;
    float maxSize = 0.45f + t;
    float count = glm::clamp(static_cast<int>(powerMW * 2.5f), 1, static_cast<int>(maxParticles));

    return std::make_tuple(minLife, maxLife, minSize, maxSize, count);
}

void ParticleSystem::emit(const glm::vec3 &sourcePos, int powerMW) {
    auto [minLife, maxLife, minSize, maxSize, count] = computeParams(powerMW);

    for (int i = 0; i < count; ++i) {
        Particle p;

        glm::vec3 posOffset = glm::vec3(
            glm::linearRand(-0.5f, 0.5f),
            glm::linearRand(-0.3f, 0.3f),
            glm::linearRand(-0.5f, 0.5f)
        );

        glm::vec3 randomWindDirection = glm::vec3(
            glm::linearRand(-1.0f, 1.0f),
            0.0f,
            glm::linearRand(-1.0f, 1.0f)
        );
        glm::vec randomJitter = glm::vec3(
            glm::linearRand(-0.3f, 0.3f),
            glm::linearRand(-0.1f, 0.1f),
            glm::linearRand(-0.3f, 0.3f)
        );
        p.position = sourcePos + posOffset;
        p.direction = glm::normalize(randomWindDirection + randomJitter);
        p.velocity = glm::linearRand(0.1f, 0.3f);
        p.life = glm::linearRand(minLife, maxLife);
        p.intensity = 50.0f;
        p.scale = glm::linearRand(minSize, maxSize);

        particles.push_back(p);

        if (particles.size() >= maxParticles) break;
    }
}

void ParticleSystem::update(float deltaTime, WindGrid& windGrid) {    
    for (auto it = particles.begin(); it != particles.end(); ) {
        adjustToWind(static_cast<Particle&>(*it), windGrid);

        it->position += (it->velocity * it->direction) * deltaTime;
        it->life -= deltaTime;
        it->intensity = it->life;

        if (it->life <= 0.0f) {
            it = particles.erase(it);
        }
        else {
            ++it;
        }
    }
}

void ParticleSystem::adjustToWind(Particle& particle, WindGrid& windGrid) {
    std::vector<WindVector> windVectors = windGrid.getWindVectorsAroundPoint(particle.position);

    if (windVectors.empty())
        return;

    glm::vec3 blendedDir(0.0f);
    float totalWeight = 0.0f;
    float accumulatedVelocity = 0.0f;

    for (const auto& windVector : windVectors) {
        float influence = calculateWindInfluence(particle, windVector);
        if (influence < 0.01f)
            continue;

        glm::vec3 windDir3D = glm::normalize(glm::vec3(windVector.direction.x, 0.0f, windVector.direction.y));
        blendedDir += windDir3D * influence;
        accumulatedVelocity += windVector.velocity * influence;
        totalWeight += influence;     
    }

    if (totalWeight < 0.01f)
        return;

    glm::vec3 newDirection = glm::normalize(blendedDir / totalWeight);
    float newVelocity = (accumulatedVelocity / totalWeight) * windVelocityScale;

    particle.direction = glm::normalize(glm::mix(particle.direction, newDirection, 0.1f));
    particle.velocity = glm::mix(particle.velocity, newVelocity, 0.1f);
}

float ParticleSystem::calculateWindInfluence(Particle& particle, const WindVector& windVector) {
    glm::vec2 particlePos = glm::vec2(particle.position.x, particle.position.z);
    glm::vec2 windPos = glm::vec2(windVector.position.x, windVector.position.z);
    float distance = glm::distance(particlePos, windPos);
    if (distance < 0.001f) 
        distance = 0.001f;
        
    float influence = (windVector.velocity) / (pow(distance, 2) + 1.0f);

    return influence;
}

void ParticleSystem::updateGPUBuffer() {
    instances.clear();
    instances.reserve(particles.size());
    for (const auto &p : particles) {
        instances.push_back({ p.position, p.intensity, p.scale });
    }
    glBindBuffer(GL_ARRAY_BUFFER, vboInstance);
    glBufferSubData(GL_ARRAY_BUFFER, 0, instances.size() * sizeof(InstanceData), instances.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::draw() {
    glDepthMask(GL_FALSE);
    updateGPUBuffer();
    glBindVertexArray(vao);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (GLsizei)instances.size());
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
}
