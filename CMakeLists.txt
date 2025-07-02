cmake_minimum_required(VERSION 3.10)
project(NuclearExplosionSim)

set(CMAKE_CXX_STANDARD 17)

set(IMGUI_SOURCES
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)

set(EXTERNAL_SOURCES
    external/glad/src/glad.c
    external/stb_image/src/stb_image.cpp
)

set(PROJECT_SOURCES
    src/main.cpp
    src/callbacks.cpp
    src/renderer.cpp
    src/model.cpp
    src/particle_system.cpp
    src/contamination.cpp
    src/gui.cpp
)

configure_file(
    configuration/root_directory.h.in 
    ${CMAKE_BINARY_DIR}/configuration/root_directory.h
)

include_directories(
    ${CMAKE_BINARY_DIR}/configuration
    external
    external/imgui
    external/imgui/backends
    external/stb_image
    external/assimp/include
    src
)

link_directories(
    ${CMAKE_SOURCE_DIR}/external/GLFW
    ${CMAKE_SOURCE_DIR}/external/assimp/lib
)

add_executable(main
    ${PROJECT_SOURCES}
    ${IMGUI_SOURCES}
    ${EXTERNAL_SOURCES}
)

target_link_libraries(main
    glfw3
    gdi32
    assimp
    opengl32
)

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/external/assimp/bin/libassimp-6.dll
    $<TARGET_FILE_DIR:main>
)
