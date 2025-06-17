#pragma once

#include <glm/gtc/matrix_transform.hpp>

class Program;

namespace Renderer {
    void renderBoxes(Program *program);
    void renderPlane(Program *program);
    glm::mat4 buildProjectionMatrix(Program *program);
    void cleanUp();
}
