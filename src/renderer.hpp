#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "model.hpp"
#include "wind_grid.hpp"

class Program;

namespace Renderer {
    void renderBoxes(Program *);
    void renderPlane(Program *);
    void renderAxis(Program *);
    void renderPlants(Program *);
    void renderPlant(Program *, glm::vec3, glm::vec3);
    void renderParticles(Program *);
    void renderWindVectors(Program *);
    void renderWindVector(Program *, WindVector&);
    glm::mat4 buildProjectionMatrix(Program *);
    void cleanUp();
}
