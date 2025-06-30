#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <vector>

const float height = 2.0f;
const glm::vec2 defaultVector = { 1.0f, 0.0f };
const float baseRadius = 2.0f;

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
        initBritoDutchVectors();
        initItalianVectors();
        initGermanVectors();
        initCzechoslovakianVectors();
        initPannonianVectors();
        initPolishVectors();
        initRomanianVectors();
        initGreekVectors();
        initUkrainianVectors();
        initBelarusianVectors();
        initRussianVectors();
        initCaspianVectors();
        initFinnishVectors();
        initNordishVectors();
        initSwedishVectors();
    }

    std::vector<WindVector> &getWindVectors() {
        return windVectors;
    }

    std::vector<WindVector> getWindVectorsAroundPoint(glm::vec3 pos) {
        glm::vec2 position = glm::vec2(pos.x, pos.z);
        std::vector<WindVector> foundVectors;
        foundVectors.reserve(8);

        for (auto &windVector : windVectors) {
            glm::vec2 windPos = glm::vec2(windVector.position.x, windVector.position.z);
            float velocityFactor;
            if (windVector.velocity < 30) {
                velocityFactor = 1.0f;
            }
            else if (windVector.velocity < 60) {
                velocityFactor = 1.25f;
            }
            else {
                velocityFactor = 1.5f;
            }
            float radius = baseRadius * velocityFactor;

            if (glm::distance(position, windPos) <= radius) {
                foundVectors.push_back(windVector);
            }
        }

        return foundVectors;
    }

private:
    void initSpanishWindVectors() {
        std::vector<WindVector> vectors = {
            WindVector(glm::vec2(0.3f, -1.0f), glm::vec2(-20.3f,  12.3f),  80.0f),
            WindVector(glm::vec2(-1.0f, -1.0f), glm::vec2(-17.56f, 15.60f), 40.0f),
            WindVector(glm::vec2(-1.0f, -0.5f), glm::vec2(-13.97f, 14.54f), 40.0f),
            WindVector(glm::vec2(-0.6f,  1.0f), glm::vec2(-16.76f, 9.46f),  10.0f),
            WindVector(glm::vec2(-1.0f,  0.7f), glm::vec2(-11.41f, 10.76f), 20.0f),
            WindVector(glm::vec2(-0.3f,  1.0f), glm::vec2(-14.14f, 9.20f),  20.0f),
            WindVector(glm::vec2(-1.0f,  0.0f), glm::vec2(-12.83f, 12.69f), 26.0f),
        };

        for (auto &windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }

    void initAfricanWindVectors() {
        std::vector<WindVector> vectors = {
            WindVector(glm::vec2(-1.0f, -0.7f), glm::vec2(-10.6f,  15.29f),  40.0f),
            WindVector(glm::vec2(-1.0f, -0.3f), glm::vec2(-14.2f,  17.71f),  40.0f),
            WindVector(glm::vec2(-1.0f,  0.1f), glm::vec2(-6.2f,  18.3f),   20.0f),
            WindVector(glm::vec2(-1.0f,  0.2f), glm::vec2(-0.39f, 17.25f),  20.0f),
            WindVector(glm::vec2(0.1f,  1.0f), glm::vec2(-1.45f, 13.99f),  20.0f),
        };

        for (auto &windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }

    void initFrenchWindVectors() {
        std::vector<WindVector> vectors = {
            WindVector(glm::vec2(1.0f, -0.20f), glm::vec2(-18.85f,  5.68f),  40.0f),
            WindVector(glm::vec2(1.0f,  0.30f), glm::vec2(-12.73f,  6.50f),  40.0f),
            WindVector(glm::vec2(1.0f, -0.10f), glm::vec2(-13.66f,  2.12f),  40.0f),
            WindVector(glm::vec2(1.0f,  1.00f), glm::vec2(-9.35f,  5.03f),  20.0f),
            WindVector(glm::vec2(1.0f,  1.00f), glm::vec2(-7.54f,  9.78f),  40.0f),
            WindVector(glm::vec2(1.0f,  0.00f), glm::vec2(-21.00f,  7.04f),  40.0f),
            WindVector(glm::vec2(1.0f, -0.20f), glm::vec2(-15.51f,  8.86f),  30.0f),
            WindVector(glm::vec2(1.0f, -0.10f), glm::vec2(-12.85f,  4.39f),  40.0f),
            WindVector(glm::vec2(1.0f, -0.05f), glm::vec2(-10.65f,  2.86f),  30.0f),
        };

        for (auto &windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }

    void initBritoDutchVectors() {
        std::vector<WindVector> vectors = {
            WindVector(glm::vec2(1.0f,  0.0f), glm::vec2(-15.80f, -3.45f),  80.0f),
            WindVector(glm::vec2(1.0f,  0.0f), glm::vec2(-19.63f, -1.60f),  40.0f),
            WindVector(glm::vec2(1.0f, -0.2f), glm::vec2(-12.56f, -0.22f),  40.0f),
            WindVector(glm::vec2(1.0f, -0.2f), glm::vec2(-6.41f, -2.32f),  40.0f),
            WindVector(glm::vec2(1.0f, -0.2f), glm::vec2(-8.27f,  0.72f),  40.0f),
        };

        for (auto &windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }

    void initItalianVectors() {
        std::vector<WindVector> vectors = {
            WindVector(glm::vec2(1.0f, -0.2f), glm::vec2(-3.84f, 9.74f),  20.0f),
            WindVector(glm::vec2(1.0f,  1.0f), glm::vec2(-3.84f, 12.06f), 20.0f),
            WindVector(glm::vec2(1.0f, -0.1f), glm::vec2(-1.53f,  6.63f), 20.0f),
            WindVector(glm::vec2(1.0f,  0.0f), glm::vec2(-1.77f,  4.90f), 20.0f),
            WindVector(glm::vec2(1.0f, -1.0f), glm::vec2(-4.76f,  3.98f), 20.0f),
            WindVector(glm::vec2(1.0f, -0.3f), glm::vec2(0.58f,  9.49f), 40.0f),
            WindVector(glm::vec2(0.0f,  1.0f), glm::vec2(3.89f, 11.75f), 40.0f),
        };

        for (auto &windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }

    void initGermanVectors() {
        std::vector<WindVector> vectors = {
            WindVector(glm::vec2(1.0f,  0.5f), glm::vec2(-2.34f,  -3.19f), 80.0f),
            WindVector(glm::vec2(1.0f, -0.4f), glm::vec2(-6.76f,  2.64f), 40.0f),
            WindVector(glm::vec2(1.0f, -0.2f), glm::vec2(-3.75f,  0.31f), 40.0f),
            WindVector(glm::vec2(1.0f,  0.0f), glm::vec2(-3.57f,  -5.10f), 40.0f),
        };

        for (auto &windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }

    void initCzechoslovakianVectors() {
        std::vector<WindVector> vectors = {
            WindVector(glm::vec2(1.0f, -0.8f), glm::vec2(0.80f, 1.64f), 20.0f),
            WindVector(glm::vec2(1.0f,  0.5f), glm::vec2(2.43f, 3.26f), 80.0f),
            WindVector(glm::vec2(1.0f,  -0.5f), glm::vec2(2.89f, 2.11f), 40.0f),
            WindVector(glm::vec2(1.0f,  -0.3f), glm::vec2(8.84f, 3.05f), 20.0f),
            WindVector(glm::vec2(1.0f,  -1.0f), glm::vec2(5.57f, 2.82f), 20.0f),
        };

        for (auto &windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }

    void initPannonianVectors() {
        std::vector<WindVector> vectors = {
            WindVector(glm::vec2(1.0f, -0.5f), glm::vec2(2.44f, 5.90f), 40.0f),
            WindVector(glm::vec2(1.0f, -1.0f), glm::vec2(3.95f, 7.16f), 20.0f),
            WindVector(glm::vec2(1.0f,  1.0f), glm::vec2(7.43f, 7.06f), 20.0f),
            WindVector(glm::vec2(1.0f,  0.4f), glm::vec2(8.36f, 5.31f), 40.0f),
        };

        for (auto &windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }

    void initPolishVectors() {
        std::vector<WindVector> vectors = {
            WindVector(glm::vec2(1.0f, -0.1f), glm::vec2(3.86f, -0.36f), 40.0f),
            WindVector(glm::vec2(1.0f,  0.0f), glm::vec2(4.68f, -2.75f), 80.0f),
            WindVector(glm::vec2(1.0f, -0.5f), glm::vec2(9.87f, -3.21f), 80.0f),
            WindVector(glm::vec2(1.0f, -0.2f), glm::vec2(9.41f,  0.16f), 40.0f),
            WindVector(glm::vec2(1.0f,  0.0f), glm::vec2(8.47f,  1.50f), 40.0f),
        };

        for (auto &windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }

    void initRomanianVectors() {
        std::vector<WindVector> vectors = {
            WindVector(glm::vec2( 1.0f, -0.1f), glm::vec2(11.14f, 6.08f), 40.0f),
            WindVector(glm::vec2( 1.0f,  1.0f), glm::vec2( 9.30f, 7.23f), 20.0f),
            WindVector(glm::vec2( 1.0f,  0.3f), glm::vec2(15.06f, 7.00f), 30.0f),
            WindVector(glm::vec2(-1.0f,  1.0f), glm::vec2(18.97f, 7.92f), 20.0f),
        };

        for (auto &windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }

    void initGreekVectors() {
        std::vector<WindVector> vectors = {
            WindVector(glm::vec2(-1.0f, 0.7f), glm::vec2(15.89f, 10.50f), 20.0f),
            WindVector(glm::vec2( 0.4f, 1.7f), glm::vec2( 9.61f, 10.85f), 20.0f),
            WindVector(glm::vec2(-0.7f, 1.0f), glm::vec2(12.76f, 11.68f), 40.0f),
            WindVector(glm::vec2(-0.7f, 1.0f), glm::vec2(10.91f, 14.33f), 40.0f),
        };

        for (auto &windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }

    void initUkrainianVectors() {
        std::vector<WindVector> vectors = {
            WindVector(glm::vec2(1.0f,  0.4f), glm::vec2(18.49f, 5.03f), 40.0f),
            WindVector(glm::vec2(1.0f,  0.0f), glm::vec2(21.90f, 5.86f), 40.0f),
            WindVector(glm::vec2(1.0f, -1.0f), glm::vec2(23.06f, 3.07f), 20.0f),
            WindVector(glm::vec2(1.0f,  0.5f), glm::vec2(25.62f, 1.80f), 20.0f),
            WindVector(glm::vec2(1.0f, -0.3f), glm::vec2(13.21f, 3.22f), 40.0f),
        };

        for (auto &windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }

    void initBelarusianVectors() {
        std::vector<WindVector> vectors = {
            WindVector(glm::vec2(1.0f,  0.2f), glm::vec2(12.58f,  1.74f), 40.0f),
            WindVector(glm::vec2(1.0f,  0.0f), glm::vec2(17.09f,  0.01f), 40.0f),
            WindVector(glm::vec2(1.0f, -1.0f), glm::vec2(13.37f, -1.04f), 40.0f),
            WindVector(glm::vec2(0.3f, -1.0f), glm::vec2(19.31f, -1.00f), 80.0f),
        };

        for (auto &windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }

    void initRussianVectors() {
        std::vector<WindVector> vectors = {
            WindVector(glm::vec2(1.0f, -1.0f), glm::vec2(16.34f, - 5.49f), 80.0f),
            WindVector(glm::vec2(0.0f, -1.0f), glm::vec2(19.01f, - 7.60f), 80.0f),
            WindVector(glm::vec2(0.5f, -1.0f), glm::vec2(21.12f, - 5.08f), 80.0f),
            WindVector(glm::vec2(0.2f, -1.0f), glm::vec2(26.02f,   1.25f), 80.0f),
            WindVector(glm::vec2(1.0f, -0.3f), glm::vec2(24.15f, - 9.85f), 40.0f),
            WindVector(glm::vec2(1.0f, -0.8f), glm::vec2(26.18f, - 6.54f), 40.0f),
            WindVector(glm::vec2(1.0f, -1.0f), glm::vec2(23.21f, - 3.50f), 40.0f),
            WindVector(glm::vec2(1.0f,  0.0f), glm::vec2(29.60f, -10.38f), 40.0f),
        };

        for (auto &windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }

    void initCaspianVectors() {
        std::vector<WindVector> vectors = {
            WindVector(glm::vec2(1.0f, 0.5f), glm::vec2(28.66f, 5.91f), 40.0f),
            WindVector(glm::vec2(1.0f, 0.5f), glm::vec2(30.27f, 3.11f), 40.0f),
            WindVector(glm::vec2(1.0f, 0.1f), glm::vec2(34.77f, 5.50f), 40.0f),
            WindVector(glm::vec2(0.2f, 1.0f), glm::vec2(35.59f, 8.85f), 40.0f),
        };

        for (auto &windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }

    void initNordishVectors() {
        std::vector<WindVector> vectors = {
            WindVector(glm::vec2( 1.0f, 0.2f), glm::vec2(-2.30f, - 8.11f), 40.0f),
            WindVector(glm::vec2( 0.1f, 1.0f), glm::vec2(-1.95f, -10.90f), 20.0f),
            WindVector(glm::vec2(-0.1f, 1.0f), glm::vec2( 1.37f, -11.55f), 20.0f),
            WindVector(glm::vec2(-0.4f, 1.0f), glm::vec2( 4.26f, -15.23f), 20.0f),
        };

        for (auto &windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }

    void initSwedishVectors() {
        std::vector<WindVector> vectors = {
            WindVector(glm::vec2( 1.0f,  0.3f), glm::vec2(1.87f, - 5.19f), 80.0f),
            WindVector(glm::vec2( 1.0f,  0.2f), glm::vec2(4.75f, - 7.40f), 50.0f),
            WindVector(glm::vec2( 1.0f, -0.5f), glm::vec2(8.55f, - 7.52f), 40.0f),
            WindVector(glm::vec2(-0.8f,  1.0f), glm::vec2(8.11f, -11.96f), 20.0f),
            WindVector(glm::vec2(-0.8f,  1.0f), glm::vec2(4.99f, -11.46f), 20.0f),
            WindVector(glm::vec2(-1.0f,  1.0f), glm::vec2(8.95f, -14.88f), 20.0f),
        };

        for (auto &windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }

    void initFinnishVectors() {
        std::vector<WindVector> vectors = {
            WindVector(glm::vec2( 1.0f, -0.3f), glm::vec2(12.20f, - 7.02f), 40.0f),
            WindVector(glm::vec2( 0.1f, -1.0f), glm::vec2(16.37f, - 8.05f), 40.0f),
            WindVector(glm::vec2(-1.0f, -1.0f), glm::vec2( 9.32f, - 9.54f), 20.0f),
            WindVector(glm::vec2(-1.0f, -0.7f), glm::vec2(13.60f, -11.39f), 20.0f),
            WindVector(glm::vec2(-1.0f, -0.6f), glm::vec2(18.98f, -12.90f), 20.0f),
            WindVector(glm::vec2(-1.0f,  0.8f), glm::vec2(14.13f, -16.51f), 20.0f),
        };

        for (auto &windVector : vectors) {
            windVectors.push_back(windVector);
        }
    }
};
