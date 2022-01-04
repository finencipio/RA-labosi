cmake_minimum_required(VERSION 3.17)
project(Labos1)

list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")

add_executable(Labos1 src/main.cpp include/mg/Vertex.h include/mg/Object.h include/mg/Triangle.h src/mg/Vertex.cpp src/mg/Triangle.cpp src/mg/Object.cpp include/animation/Trajectory.h src/animation/Trajectory.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include")


set(CMAKE_CXX_STANDARD 14)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )


target_link_libraries(${PROJECT_NAME} GL GLU glfw3 X11 Xxf86vm Xrandr pthread Xi dl Xinerama Xcursor assimp)
