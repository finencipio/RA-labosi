cmake_minimum_required(VERSION 3.15)
project(Projekt)

set(CMAKE_CXX_STANDARD 20)

set(OpenGlLinkers -lGLU -lGL -lGLEW)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_package(GLEW REQUIRED STATIC)

file(COPY shaders DESTINATION ${PROJECT_BINARY_DIR})

add_executable(Projekt src/main.cpp src/Display.cpp include/Display.h src/Loader.cpp include/Loader.h src/Shader.cpp include/Shader.h)

target_link_libraries(Projekt ${OpenGlLinkers}) # linking opengl libraries to the project

include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(Projekt ${GLFW_LIBRARIES})
target_link_libraries(Projekt
        -ldl
        -pthread
        ${OPENGL_LIBRARIES}
        ${GLFW3_LIBRARY}
        ${GLEW_LIBRARY})