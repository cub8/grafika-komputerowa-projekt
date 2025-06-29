#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <vector>

const float height = 2.0f;
const glm::vec2 defaultVector = { 1.0f, 0.0f };
const float radius = 3.0f;

struct WindVector {
    glm::vec2 direction; // x and z direction
    glm::vec3 position;
    float velocity;

    // Note that pos is 2 dimensional point, where y is actually z.
    WindVector(glm::vec2 dir, glm::vec2 pos, float vel) :
        direction(dir), position(pos.x, height, pos.y), velocity(vel) {}

    float getAngleRadians() {
        glm::vec2 normalizedDirection = glm::normalize(glm::vec2(direction.x, -direction.y));
        glm::vec2 normalizedDefaultVector = glm::normalize(defaultVector);
        
        float dot = glm::dot(normalizedDefaultVector, normalizedDirection);
        float det = normalizedDefaultVector.x * normalizedDirection.y - normalizedDefaultVector.y * normalizedDirection.x;

        return atan2(det, dot);
    }

    glm::vec3 getVectorColor() {
        if (velocity < 30) {
            return glm::vec3(0.0f, 0.0f, 1.0f);
        }
        else if (velocity < 60) {
            return glm::vec3(1.0f, 1.0f, 0.0f);
        }
        else {
            return glm::vec3(1.0f, 0.0f, 0.0f);
        }
    }

    float getSpeedFactor() {
        if (velocity < 30) {
            return 0.5f;
        }
        else if (velocity < 60) {
            return 1.0f;
        }
        else {
            return 2.0f;
        }
    }
};

class WindGrid {
    std::vector<WindVector> windVectors;

public:
    WindGrid() {
        windVectors.reserve(16);
    }

    void initialize() {
        initSpanishWindVectors();
        initAfricanWindVectors();
        initFrenchWindVectors();
    }

    std::vector<WindVector>& getWindVectors() {
        return windVectors;
    }

    std::vector<WindVector> getWindVectorsAroundPoint(glm::vec3 pos) {
        glm::vec2 position = glm::vec2(pos.x, pos.z);
        std::vector<WindVector> foundVectors;
        foundVectors.reserve(8);

        for (auto& windVector : windVectors) {
            glm::vec2 windPos = glm::vec2(windVector.position.x, windVector.position.z);

            if (glm::distance(position, windPos) <= radius) {
                foundVectors.push_back(windVector);
            }
        }

        return foundVectors;
    }

private:
    void initSpanishWindVectors() {
        std::array<WindVector, 5> vectors = {
            WindVector(glm::vec2( 0.3f, -1.0f), glm::vec2(-20.3f,  12.3f),  80.0f),
            WindVector(glm::vec2(-1.0f, -1.0f), glm::vec2(-17.56f, 15.60f), 40.0f),
            WindVector(glm::vec2(-1.0f, -0.5f), glm::vec2(-13.97f, 14.54f), 40.0f),
            WindVector(glm::vec2(-0.6f,  1.0f), glm::vec2(-16.76f, 9.46f),  20.0f),
            WindVector(glm::vec2(-1.0f,  0.7f), glm::vec2(-11.41f, 10.76f),  20.0f),
        };

        for (auto& windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }

    void initAfricanWindVectors() {
        std::array<WindVector, 5> vectors = {
            WindVector(glm::vec2(-1.0f, -0.7f), glm::vec2(-10.6f,  15.29f),  40.0f),
            WindVector(glm::vec2(-1.0f, -0.3f), glm::vec2(-14.2f,  17.71f),  40.0f),
            WindVector(glm::vec2(-1.0f, -0.8f), glm::vec2( -6.2f,  18.3f),   20.0f),
            WindVector(glm::vec2(-1.0f,  0.2f), glm::vec2( -0.39f, 17.25f),  20.0f),
            WindVector(glm::vec2( 0.1f,  1.0f), glm::vec2( -1.45f, 13.99f),  20.0f),
        };

        for (auto& windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }

    void initFrenchWindVectors() {
        std::array<WindVector, 5> vectors = {
            WindVector(glm::vec2(1.0f, -0.20f), glm::vec2(-18.85f,  5.68f),  40.0f),
            WindVector(glm::vec2(1.0f, -1.00f), glm::vec2(-12.73f,  6.50f),  40.0f),
            WindVector(glm::vec2(1.0f, -0.10f), glm::vec2(-13.66f,  2.12f),  40.0f),
            WindVector(glm::vec2(1.0f,  1.00f), glm::vec2( -9.35f,  5.03f),  20.0f),
            WindVector(glm::vec2(1.0f, -0.05f), glm::vec2( -7.54f,  9.78f),  40.0f),
        };

        for (auto& windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }
};
