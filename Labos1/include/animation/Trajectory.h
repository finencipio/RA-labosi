#ifndef RA_1_TRAJECTORY_H
#define RA_1_TRAJECTORY_H

#include <glm/glm.hpp>
#include <vector>
#include "mg/Vertex.h"

namespace ra {
    namespace animation {
        class Trajectory {
        private:
            std::vector<mg::Vertex> rs;
            std::vector<glm::vec3> ps;
            glm::mat4x4 B = glm::transpose(glm::mat4x4(
                    glm::vec4(-1,3,-3,1),
                    glm::vec4(3, -6, 3, 0),
                    glm::vec4(-3, 0, 3, 0),
                    glm::vec4(1, 4, 1, 0)
                    ));
            float Bscale = 1.0 / 6;
            glm::mat4x3 dB = glm::transpose(glm::mat3x4(
                    glm::vec4(-1, 3, -3, 1),
                    glm::vec4(2, -4, 2, 0),
                    glm::vec4(-1, 0, 1, 0)
                    ));
            float dBscale = 0.5;
            glm::mat4x2 ddB = glm::transpose(glm::mat2x4(
                    glm::vec4(-1, 3, -3, 1),
                    glm::vec4(1, -2, 1, 0)
                    ));
            float ddBscale = 1;

            glm::mat3x4 getRMatrix(int i);
            void calculatePath();
        public:
            Trajectory(std::string filepath);
            glm::vec3 p(double t, int i);
            glm::vec3 dp(double t, int i);
            glm::vec3 ddp(double t, int i);
            void draw();
            int segmentCount() {return rs.size() - 3;}
        };
    }
}

#endif //RA_1_TRAJECTORY_H
