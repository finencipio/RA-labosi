#ifndef RA_1_OBJECT_H
#define RA_1_OBJECT_H

#include "vector"
#include "Triangle.h"
#include <fstream>
#include "animation/Trajectory.h"

namespace ra {
    namespace mg {
        class Object {
        private:
            std::vector<Triangle> triangles;
            std::vector<Vertex> vertices;
            glm::vec3 scaleVec;
            glm::vec3 center = glm::vec3(0,0,0);
            void drawWithDMCRotation(animation::Trajectory &trajectory, double t, int segment);
            void drawWithAxisRotation(animation::Trajectory &trajectory, double t, int segment);
        public:
            Object(std::string filePath);
            void draw(animation::Trajectory &trajectory, double t, int segment);
        };
    }
}

#endif //RA_1_OBJECT_H
