#pragma once
#include <GLFW/glfw3.h>
struct GLFWwindow;
class Program;

namespace Gui {
    void initialize(GLFWwindow* window);
    void shutdown();
    void beginFrame();
    void endFrame();
    void render(Program* program);
}
