cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

# === IMGUI SOURCES ===
set(IMGUI_SOURCES
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)

# === POZOSTAŁE EXTERNAL SOURCES (tylko te potrzebne) ===
set(EXTERNAL_SOURCES
    external/glad/src/glad.c
    external/stb_image/src/stb_image.cpp
)

# === TWOJE ŹRÓDŁA PROJEKTU ===
set(SOURCES
    src/main.cpp
    src/callbacks.cpp
    src/renderer.cpp
    src/model.cpp
    src/particle_system.cpp
    src/gui.cpp
)

# === ROOT HEADER ===
configure_file(
    configuration/root_directory.h.in 
    ${CMAKE_BINARY_DIR}/configuration/root_directory.h
)

# === INCLUDE PATHS ===
include_directories(
    ${CMAKE_BINARY_DIR}/configuration
    external
    src
    external/assimp/include
    external/imgui
    external/imgui/backends
    external/stb_image
)

# === LINK PATHS ===
link_directories(
    ${CMAKE_SOURCE_DIR}/external/GLFW
    ${CMAKE_SOURCE_DIR}/external/assimp/lib
)

# === EXECUTABLE ===
add_executable(main
    ${SOURCES}
    ${IMGUI_SOURCES}
    ${EXTERNAL_SOURCES}
)

# === LINK LIBRARIES ===
target_link_libraries(main glfw3 gdi32 assimp)

# === DLL COPY (Assimp) ===
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/external/assimp/bin/libassimp-6.dll
    $<TARGET_FILE_DIR:main>
)
