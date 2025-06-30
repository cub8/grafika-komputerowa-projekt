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
        Shader& shader = program->getBoxShader();
        Object& object = program->getObject();

        program->getTexture1().bindTexture(GL_TEXTURE0);
        program->getTexture2().bindTexture(GL_TEXTURE1);
        shader.use();

        glm::mat4 view = program->getCamera().GetViewMatrix();
        shader.setMat4("view", view);
        shader.setMat4("projection", buildProjectionMatrix(program));

        object.bindVertexArray();
        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, program->cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);
            object.draw();
        }

        cleanUp();
    }

    void renderPlane(Program *program) {
        Shader& shader = program->getPlaneShader();
        Object& plane = program->getPlane();

        // slot 0 - map
        glActiveTexture(GL_TEXTURE0);
        program->getTexture3().bindTexture(GL_TEXTURE0);
        shader.setInt("Tex", 0);

        // slot 1 - contamination
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
        Shader& shader = program->getAxisShader();
        Object& axis = program->getAxis();

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
        for (int i = 0; i < program->plantPositions.size(); ++i) {
            program->getModelShader().use();
            auto &pos = program->plantPositions[i];

            if (program->getSelectedPlantIndex() == i) {
                program->getModelShader().setVec3("overrideColor1", glm::vec3(1.f, 0.f, 0.f)); // red color if selected
            }
            else {
                program->getModelShader().setVec3("overrideColor1", glm::vec3(-1.f)); // no color
            }

            renderPlant(program, pos, glm::vec3(0.003f));
        }
    }

    void renderPlant(Program* program, glm::vec3 position, glm::vec3 scale) {
        Shader& shader = program->getModelShader();
        Model& plant = program->getPowerPlantModel(); 
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
        program->particleSystem.update(program->deltaTime);

        glm::mat4 view       = program->camera.GetViewMatrix();
        glm::mat4 projection = buildProjectionMatrix(program);

        // 2D ground contamination     

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

        // 3D particles

        program->getPsTexture().bindTexture(GL_TEXTURE0);
        program->getParticleShader().use();

        program->getParticleShader().setMat4("view", view);
        program->getParticleShader().setMat4("projection", projection);

        program->particleSystem.draw();
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
