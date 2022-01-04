#include "../include/Display.h"
#include "../include/Loader.h"
#include "../include/Shader.h"
#include "../include/Camera.h"
#include <map>

Camera camera;

bool pressed = false;

glm::vec2 startPosRotation = glm::vec2(0,0);
std::map<int, bool> pressedKeyMap = {{GLFW_KEY_W, false}, {GLFW_KEY_A, false},
                                     {GLFW_KEY_S, false}, {GLFW_KEY_D, false},
                                     {GLFW_KEY_SPACE, false}, {GLFW_KEY_P, false}};

float power = 8.0;
void myKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(pressed) {
        camera.rotateOnXAndY(ypos-startPosRotation.y, xpos - startPosRotation.x);

        startPosRotation = glm::vec2(xpos, ypos);
    }
}

void loadMandelbulb(Shader &shader)
{
    shader.loadPow(power);
    shader.loadResolution(glm::vec2{Display::getWidth(), Display::getHeight()});
    shader.loadCameraPosition(camera.cameraPosition);
    shader.loadCameraRotation(camera.cameraRotation);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        pressed = true;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        startPosRotation = glm::vec2(xpos, ypos);
    }
    else
        pressed = false;
}

void move() {

    float factor = pressedKeyMap[GLFW_KEY_SPACE] ? 0.01f : 0.005f;

    if(pressedKeyMap[GLFW_KEY_W]) {
        camera.moveForward(factor);
    }

    if(pressedKeyMap[GLFW_KEY_S]) {
        camera.moveBack(factor);
    }

    if(pressedKeyMap[GLFW_KEY_A]) {
        camera.moveLeft(factor);
    }

    if(pressedKeyMap[GLFW_KEY_D]) {
        camera.moveRight(factor);
    }
}

int main()
{
    constexpr int width = 800;
    constexpr int height = 450;

    Display display(width, height, "Mandelbulb");

    glfwSetKeyCallback(display.window, myKeyboard);
    glfwSetCursorPosCallback(display.window, cursor_position_callback);
    glfwSetMouseButtonCallback(display.window, mouse_button_callback);

    std::vector<float> vertices = {-1, 1, -1, -1, 1, -1, 1, 1};
    std::vector<unsigned> indices = {0, 1, 2, 0, 2, 3};

    Loader loader;

    Model model = loader.loadToVao(vertices, indices);
    Shader shader;

    shader.start();

    // TODO do not forget to change FragmentShader file in Shader.h
    loadMandelbulb(shader);

    Shader::stop();

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLineWidth(30);

    double temp, previous = glfwGetTime();

    while (display.isRunning())
    {
        temp = glfwGetTime();
        if(temp - previous < 0.02) {
            continue;
        }
        previous = temp;

        if(pressedKeyMap[GLFW_KEY_P]) {
            power = 8 + 6 * sin(temp / 4);
        }
        Display::clearWindow();

        shader.start();
        glBindVertexArray(model.vaoId);
        loader.bindVao();

        move();
        loadMandelbulb(shader);

        glDrawElements(GL_TRIANGLES, model.vertexCount, GL_UNSIGNED_INT, nullptr);

        loader.unbindVao();
        Shader::stop();
        display.update();
    }

    return 0;
}

void myKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(!pressedKeyMap.contains(key)) {
        return;
    }
    pressedKeyMap[key] = action != GLFW_RELEASE;
}
