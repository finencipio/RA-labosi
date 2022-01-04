#include "../include/Display.h"

int Display::w;
int Display::h;

Display::Display(int width, int height, const char *title)
{
    w = width;
    h = height;

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewInit();

    glfwSetWindowSizeCallback(window, windowResizeCallback);
}

Display::~Display()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Display::update()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Display::isRunning() const
{
    return !glfwWindowShouldClose(window);
}

void Display::clearWindow()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Display::windowResizeCallback(GLFWwindow *window, int width, int height)
{
    w = width;
    h = height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1.5, 1.5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int Display::getWidth()
{
    return w;
}

int Display::getHeight()
{
    return h;
}
