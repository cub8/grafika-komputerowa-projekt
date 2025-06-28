#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "model.hpp"

class Program;

namespace Renderer {
    void renderBoxes(Program *program);
    void renderPlane(Program *program);
    void renderAxis(Program *program);
    void renderPlants(Program *program);
    void renderPlant(Program* program, glm::vec3 position, glm::vec3 scale);
    void renderParticles(Program* program);
    glm::mat4 buildProjectionMatrix(Program *program);
    void cleanUp();
}
