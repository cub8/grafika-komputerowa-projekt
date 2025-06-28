#include "program.hpp"

int main() {
    try {
        Program program("Nuclear Power Plants");
        program.renderLoop();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        std::cin.get(); // poczekaj na Enter
    }
    return 0;
}
