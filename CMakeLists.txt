cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE EXTERNAL_SOURCES
    ${CMAKE_SOURCE_DIR}/external/*.c
    ${CMAKE_SOURCE_DIR}/external/*.cpp
)

set(SOURCES
    src/main.cpp
    src/callbacks.cpp
    src/renderer.cpp
    src/model.cpp
    src/particle_system.cpp
    ${EXTERNAL_SOURCES}
    ${INTERNAL_SOURCES}
)
configure_file(
    configuration/root_directory.h.in 
    ${CMAKE_BINARY_DIR}/configuration/root_directory.h
    )

# Include directories
include_directories(
    ${CMAKE_BINARY_DIR}/configuration
    external 
    src
    external/assimp/include
)

# Link GLFW (static)
link_directories(
    ${CMAKE_SOURCE_DIR}/external/GLFW
    ${CMAKE_SOURCE_DIR}/external/assimp/lib
    )

add_executable(main ${SOURCES})

# Link dependencies
target_link_libraries(main glfw3 gdi32 assimp)

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/external/assimp/bin/libassimp-6.dll
    $<TARGET_FILE_DIR:main>
)
