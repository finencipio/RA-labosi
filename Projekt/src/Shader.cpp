#include "../include/Shader.h"

#include <fstream>
#include <string>
#include <iostream>

Shader::Shader()
{
    vertexShader = loadShader(VERTEX_FILE, GL_VERTEX_SHADER);
    fragmentShader = loadShader(FRAGMENT_FILE, GL_FRAGMENT_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    getUniformLocations();
}

GLuint Shader::loadShader(const char *filename, GLenum type)
{
    std::ifstream reader(filename);
    std::string source((std::istreambuf_iterator<char>(reader)),
                       std::istreambuf_iterator<char>());

    const char *shaderSource = source.c_str();

    GLuint shaderId = glCreateShader(type);
    glShaderSource(shaderId, 1, &shaderSource, nullptr);
    glCompileShader(shaderId);

    debug(shaderId);

    reader.close();
    return shaderId;
}

void Shader::debug(GLuint shaderId)
{
    int result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *) alloca(length * sizeof(char));

        glGetShaderInfoLog(shaderId, length, &length, message);

        std::cout << "Error while compiling shader: " << message << "\n";
        glDeleteShader(shaderId);
    }
}

void Shader::start()
{
    glUseProgram(program);
}

void Shader::stop()
{
    glUseProgram(0);
}

void Shader::getUniformLocations()
{
    locationCameraPosition = glGetUniformLocation(program, "cameraPosition");
    locationResolution = glGetUniformLocation(program, "resolution");
    locationCameraRotation = glGetUniformLocation(program, "cameraRotation");
    locationPow = glGetUniformLocation(program, "power");
}

Shader::~Shader()
{
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glDeleteProgram(program);
}

void Shader::loadCameraPosition(const glm::vec3 &position)
{
    glUniform3f(locationCameraPosition, position.x, position.y, position.z);
}

void Shader::loadCameraRotation(const glm::mat3 &cameraRotation)
{
    glUniformMatrix3fv(locationCameraRotation, 1, GL_FALSE, &cameraRotation[0][0]);
}

void Shader::loadResolution(const glm::vec2 &resolution)
{
    glUniform2f(locationResolution, resolution.x, resolution.y);
}

void Shader::loadPow(const float &power)
{
    glUniform1f(locationPow, power);
}
