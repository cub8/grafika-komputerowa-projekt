#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "gui.hpp"
#include "program.hpp"

void Gui::initialize(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Gui::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Gui::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Gui::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::render(Program* program) {
    ImGui::Begin("BOOOM!");

    if (program->selectedPlantIndex && *program->selectedPlantIndex >= 0) {
        const auto& plant = program->nuclearPowerPlants[*program->selectedPlantIndex];
        const auto& pos = plant.position;

        ImGui::Text("Zaznaczona elektrownia:");
        ImGui::BulletText("Index: %d", *program->selectedPlantIndex);
        ImGui::BulletText("Pozycja: (x=%.2f, y=%.2f, z=%.2f)", pos.x, pos.y, pos.z);
        ImGui::BulletText("Moc: %.1f MW", plant.powerMW);

        if (ImGui::Button("Explosion")) {
            program->particleSystem.emit(pos + glm::vec3(0, 2.5f, 0), plant.powerMW);
            program->contaminationMask.initialize(program->SCR_WIDTH, program->SCR_HEIGHT);
            program->contaminationMask.clear();

            std::cout << "Wysadzono elektrownię: " << *program->selectedPlantIndex << "\n";
        }
    } else {
        ImGui::Text("Nie wybrano żadnej elektrowni.");
    }

    ImGui::End();
}
