#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.hpp"
#include "object.hpp"
#include "program.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "world_constraints.hpp"

namespace Renderer {
    void renderBoxes(Program *program) {
        Shader &shader = program->getBoxShader();
        Object &box = program->getBox();

        program->getTexture1().bindTexture(GL_TEXTURE0);
        program->getTexture2().bindTexture(GL_TEXTURE1);
        shader.use();

        glm::mat4 view = program->getCamera().GetViewMatrix();
        shader.setMat4("view", view);
        shader.setMat4("projection", buildProjectionMatrix(program));

        glm::vec3 position = program->getCamera().Position;

        box.bindVertexArray();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position.x, 0.0f, position.z));
        model = glm::scale(model, glm::vec3(0.25, 0.25f, 0.25f));
        shader.setMat4("model", model);
        box.draw();
        cleanUp();
    }

    void renderPlane(Program *program) {
        Shader &shader = program->getPlaneShader();
        Object &plane = program->getPlane();

        glActiveTexture(GL_TEXTURE0);
        program->getTexture3().bindTexture(GL_TEXTURE0);
        shader.setInt("Tex", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, program->contaminationMask.getTextureID());
        shader.setInt("ContaminationTex", 1);

        shader.use();

        auto view = program->getCamera().GetViewMatrix();
        shader.setMat4("view", view);
        shader.setMat4("projection", buildProjectionMatrix(program));
        
        auto model = glm::mat4(1.0f);
        auto modelScale = glm::vec3(WorldConstraints::SCALE, 1.0f, WorldConstraints::SCALE);
        model = glm::scale(model, modelScale);
        shader.setMat4("model", model);

        plane.bindVertexArray();
        plane.draw();

        cleanUp();
    }

    void renderAxis(Program *program) {
        Shader &shader = program->getAxisShader();
        Object &axis = program->getAxis();

        shader.use();

        auto view = program->getCamera().GetViewMatrix();
        shader.setMat4("view", view);
        shader.setMat4("projection", buildProjectionMatrix(program));
        axis.bindVertexArray();

        auto model = glm::mat4(1.0f);
        auto modelScale = glm::vec3(30.0f, 30.0f, 30.0f);
        model = glm::scale(model, modelScale);
        shader.setMat4("model", model);
        axis.drawLines();

        cleanUp();
    }

    void renderPlants(Program* program) {
        auto& plants = program->nuclearPowerPlants;
        for (int i = 0; i < plants.size(); ++i) {
            program->getModelShader().use();
            const glm::vec3& pos = plants[i].position;

            if (program->getSelectedPlantIndex() == i) {
                program->getModelShader().setVec3("overrideColor1", glm::vec3(1.f, 0.f, 0.f)); // red color if selected
            }
            else {
                program->getModelShader().setVec3("overrideColor1", glm::vec3(-1.f)); // no color
            }

            renderPlant(program, pos, glm::vec3(0.003f));
        }
    }

    void renderPlant(Program *program, glm::vec3 position, glm::vec3 scale) {
        Shader &shader = program->getModelShader();
        Model &plant = program->getPowerPlantModel();
        shader.use();

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::scale(modelMatrix, scale);
        shader.setMat4("model", modelMatrix);

        glm::mat4 view = program->getCamera().GetViewMatrix();
        glm::mat4 projection = buildProjectionMatrix(program);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        plant.Draw(shader);
        cleanUp();
    }

    void renderParticles(Program *program) {
        program->getPsTexture().bindTexture(GL_TEXTURE0);
        program->getParticleShader().use();

        glm::mat4 view       = program->camera.GetViewMatrix();
        glm::mat4 projection = buildProjectionMatrix(program);

        float realLEFT = -WorldConstraints::SCALE * WorldConstraints::ASPECT_RATIO;
        float realRIGHT = WorldConstraints::SCALE * WorldConstraints::ASPECT_RATIO;
        float realBOTTOM = WorldConstraints::SCALE;
        float realTOP = -WorldConstraints::SCALE;

        glm::mat4 orthoProj = glm::ortho(
            realLEFT, realRIGHT,
            realBOTTOM, realTOP,
            -1.0f, 1.0f);

        glm::mat4 orthoView = glm::mat4(1.0f);

        program->getContaminationShader().use();
        program->contaminationMask.bind();
        
        program->getContaminationShader().setMat4("view", orthoView);
        program->getContaminationShader().setMat4("projection", orthoProj);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        program->particleSystem.draw();

        program->contaminationMask.unbind();

        program->getPsTexture().bindTexture(GL_TEXTURE0);
        program->getParticleShader().use();

        program->getParticleShader().setMat4("view", view);
        program->getParticleShader().setMat4("projection", projection);

        program->particleSystem.draw();
        cleanUp();
    }

    void renderWindVectors(Program *program) {
        if (!program->renderWindVectors)
            return;

        for (auto &windVector : program->getWindGrid().getWindVectors()) {
            renderWindVector(program, windVector);
        }
    }

    void renderWindVector(Program *program, WindVector &windVector) {
        Shader &shader = program->getWindVectorShader();
        Object &vectorArrow = program->getVectorArrow();

        shader.use();
        glm::mat4 view = program->getCamera().GetViewMatrix();
        glm::mat4 projection = buildProjectionMatrix(program);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        float angle = windVector.getAngleRadians();
        float speedFactor = windVector.getSpeedFactor();
        float time = speedFactor * glfwGetTime();
        float fractPart = time - static_cast<int>(time);
        float moveFactor = 2.0f;

        glm::vec3 direction = glm::vec3(windVector.direction.x, 0.0f, windVector.direction.y);

        glm::mat4 vectorArrowModel = glm::mat4(1.0f);
        vectorArrowModel = glm::translate(vectorArrowModel, windVector.position);
        vectorArrowModel = glm::translate(vectorArrowModel, moveFactor * fractPart * direction);
        vectorArrowModel = glm::rotate(vectorArrowModel, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        vectorArrowModel = glm::scale(vectorArrowModel, glm::vec3(1.0f, 1.0f, 0.5f));

        shader.setFloat("alpha", 1.0f - fractPart);
        shader.setMat4("model", glm::scale(vectorArrowModel, glm::vec3(1.05f, 1.0f, 1.05f)));
        shader.setVec3("color", glm::vec3(1.0f));
        vectorArrow.bindVertexArray();
        vectorArrow.draw();

        shader.setMat4("model", glm::translate(vectorArrowModel, glm::vec3(0.0f, 0.01f, 0.0f)));
        shader.setVec3("color", windVector.getVectorColor());
        vectorArrow.draw();

        cleanUp();
    }

    glm::mat4 buildProjectionMatrix(Program *program) {
        const float RATIO = program->getAspectRatio();
        const float FOV = glm::radians(program->getFov());

        return glm::perspective(FOV, RATIO, 0.1f, 100.0f);
    }

    void cleanUp() {
        glUseProgram(0);
        glBindVertexArray(0);
    }
}
