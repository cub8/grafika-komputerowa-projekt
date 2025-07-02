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

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
    ImGui::Begin("Controls");

if (ImGui::Button("Show Wind Vectors")) {
    program->renderWindVectors = true;
}

if (ImGui::Button("Hide Wind Vectors")) {
    program->renderWindVectors = false;
}

if (ImGui::Button("Clear Contamination")) {
    program->contaminationMask.clear();
}

ImGui::End();

ImGui::SetNextWindowPos(ImVec2(10, 110), ImGuiCond_Once);
ImGui::Begin("BOOOM!");

int selectedIndex = program->selectedPlantIndex.value_or(-1);

if (selectedIndex >= 0 && selectedIndex < program->nuclearPowerPlants.size()) {
    const auto& plant = program->nuclearPowerPlants[selectedIndex];
    const std::string& name = program->plantNames[selectedIndex];

    ImGui::Text("Selected Plant: %s", name.c_str());
    ImGui::Text("Power: %.1f MW", plant.powerMW);

    ImGui::SliderFloat("Set Power", &program->nuclearPowerPlants[selectedIndex].powerMW, 500.0f, 8000.0f);
} else {
    ImGui::Text("No plant selected");
}

ImVec2 bigButtonSize(300, 100); 
if (ImGui::Button("Explosion", bigButtonSize) && selectedIndex >= 0 && selectedIndex < program->nuclearPowerPlants.size()) {
    auto& plant = program->nuclearPowerPlants[selectedIndex];
    glm::vec3 pos = plant.position;
    program->particleSystem.emit(pos + glm::vec3(0, 2.5f, 0), plant.powerMW);
    program->contaminationMask.initialize(program->SCR_WIDTH, program->SCR_HEIGHT);
}

ImGui::End();


}
