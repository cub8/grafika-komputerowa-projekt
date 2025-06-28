#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "model.hpp"

class Program;

namespace Renderer {
    void renderBoxes(Program *program);
    void renderPlane(Program *program);
    void renderAxis(Program *program);
    void renderModel(Program* program, Model& modelObject, glm::vec3 position, glm::vec3 scale);
    glm::mat4 buildProjectionMatrix(Program *program);
    void cleanUp();
}
