#ifndef LAB3_DISPLAY_H
#define LAB3_DISPLAY_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

class Display
{
private:

    static int w,h;

    static void windowResizeCallback(GLFWwindow *window, int width, int height);

public:
    Display(int width, int height, const char *title);

    ~Display();

    void update();

    [[nodiscard]] bool isRunning() const;

    static void clearWindow();

    static int getWidth();

    static int getHeight();
    GLFWwindow *window;
};


#endif //LAB3_DISPLAY_H
