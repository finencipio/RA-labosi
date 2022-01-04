#ifndef LAB3_SHADER_H
#define LAB3_SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader
{
private:
    constexpr static const char *VERTEX_FILE = "./shaders/VertexShader.glsl";
    constexpr static const char *FRAGMENT_FILE = "./shaders/mandelbulb.glsl";

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint program;

    GLuint locationCameraPosition{};
    GLuint locationCameraRotation{};
    GLuint locationResolution{};
    GLuint locationPow{};

    static GLuint loadShader(const char *filename, GLenum type);

    static void debug(GLuint shaderId);

    void getUniformLocations();

public:
    Shader();

    ~Shader();

    void start();

    static void stop();

    void loadCameraPosition(const glm::vec3 &position);

    void loadCameraRotation(const glm::mat3 &cameraRotation);

    void loadResolution(const glm::vec2 &resolution);

    void loadPow(const float &pow);
};


#endif //LAB3_SHADER_H
