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
        program->getTexture1().bindTexture(GL_TEXTURE0);
        program->getTexture2().bindTexture(GL_TEXTURE1);
        program->getBoxShader().use();

        glm::mat4 view = program->getCamera().GetViewMatrix();
        program->getBoxShader().setMat4("view", view);
        program->getBoxShader().setMat4("projection", buildProjectionMatrix(program));

        program->getObject().bindVertexArray();
        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, program->cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            program->getBoxShader().setMat4("model", model);
            program->getObject().draw();
        }

        cleanUp();
    }

    void renderPlane(Program *program) {
        program->getTexture3().bindTexture(GL_TEXTURE0);
        program->getPlaneShader().use();

        auto view = program->getCamera().GetViewMatrix();
        program->getPlaneShader().setMat4("view", view);
        program->getPlaneShader().setMat4("projection", buildProjectionMatrix(program));

        program->getPlane().bindVertexArray();
        auto model = glm::mat4(1.0f);
        auto modelScale = glm::vec3(WorldConstraints::SCALE, 1.0f, WorldConstraints::SCALE);
        model = glm::scale(model, modelScale);
        program->getPlaneShader().setMat4("model", model);
        program->getPlane().draw();

        cleanUp();
    }

    void renderAxis(Program *program) {
        program->getAxisShader().use();

        auto view = program->getCamera().GetViewMatrix();
        program->getAxisShader().setMat4("view", view);
        program->getAxisShader().setMat4("projection", buildProjectionMatrix(program));
        program->getAxis().bindVertexArray();
        
        auto model = glm::mat4(1.0f);
        auto modelScale = glm::vec3(30.0f, 30.0f, 30.0f);
        model = glm::scale(model, modelScale);
        program->getAxisShader().setMat4("model", model);
        program->getAxis().drawLines();

        cleanUp();
    }


    void renderModel(Program* program, Model& modelObject, glm::vec3 position, glm::vec3 scale) {

        Shader& shader = program->getModelShader();
        shader.use();

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::scale(modelMatrix, scale);
        shader.setMat4("model", modelMatrix);

        // View + projection
        glm::mat4 view = program->getCamera().GetViewMatrix();
        glm::mat4 projection = buildProjectionMatrix(program);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        modelObject.Draw(shader);

        cleanUp();
    }   



    glm::mat4 buildProjectionMatrix(Program *program) {
        const float SCR_WIDTH = static_cast<float>(program->SCR_WIDTH);
        const float SCR_HEIGHT = static_cast<float>(program->SCR_HEIGHT);
        const float RATIO = SCR_WIDTH / SCR_HEIGHT;
        const float FOV = glm::radians(program->getCamera().Zoom);

        return glm::perspective(FOV, RATIO, 0.1f, 100.0f);
    }

    void cleanUp() {
        glUseProgram(0);
        glBindVertexArray(0);
    }
}
