#ifndef LAB4_CAMERA_H
#define LAB4_CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    glm::vec3 cameraPosition = glm::vec3(-1.39985,0.000284176,-1.89639);
    glm::mat3 cameraRotation = glm::mat3({{-0.822496,0.00692783,0.568728},
                                          {0.028768,0.999153,0.0294334},
                                          {-0.568042,0.0405701,-0.821999}});

    Camera() {}

    glm::mat3 normMat3(glm::mat3 &matrix) {
        matrix[0] = glm::normalize(glm::cross(matrix[1], matrix[2]));
        matrix[1] = glm::normalize(glm::cross(matrix[2], matrix[0]));
        matrix[2] = glm::normalize(glm::cross(matrix[0], matrix[1]));

        return matrix;
    }

    void rotateOnXAndY(double unitsY, double unitsX) {
        double c = cos(-0.005 * unitsY);
        double s = sin(-0.005 * unitsY);
        cameraRotation *= glm::mat3({{1, 0, 0},
                                     {0,c,s},
                                     {0,-s,c}});

        c = cos(0.005 * unitsX);
        s = sin(0.005 * unitsX);
        cameraRotation *= glm::mat3({{c, 0, -s},
                                     {0,1,0},
                                     {s,0,c}});

        normMat3(cameraRotation);
    }

    void moveForward(float factor) {
        cameraPosition -= factor * cameraRotation[2];
    }

    void moveBack(float factor) {
        cameraPosition += factor * cameraRotation[2];
    }

    void moveRight(float factor) {
        cameraPosition -= factor * cameraRotation[0];
    }

    void moveLeft(float factor) {
        cameraPosition += factor * cameraRotation[0];
    }
};

#endif //LAB4_CAMERA_H
