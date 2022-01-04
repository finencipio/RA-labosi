#include <string>
#include <animation/Trajectory.h>
#include <fstream>
#include <sstream>
#include <GL/glut.h>

namespace ra {
    namespace animation {
        Trajectory::Trajectory(std::string filePath) {
            std::ifstream file(filePath);
            if(!file) {
                std::cerr << "Trajectory file could not be read, source: " << filePath << std::endl;
            }
            std::string line;
            while(getline(file, line)) {
                double x,y,z;
                std::istringstream iss(line);
                iss >> x >> y >> z;
                rs.push_back(mg::Vertex(x,y,z));
            }

            calculatePath();
        }

        glm::mat3x4 Trajectory::getRMatrix(int i) {
            mg::Vertex r0 = rs[i-1], r1 = rs[i], r2 = rs[i+1], r3 = rs[i+2];
            return glm::transpose(glm::mat4x3(r0, r1, r2, r3));
        }

        glm::vec3 Trajectory::p(double t, int i) {
            glm::vec4 T = glm::vec4(pow(t,3), pow(t,2), t, 1);
            glm::mat3x4 R = getRMatrix(i);

            return T * Bscale * B * R;
        }

        glm::vec3 Trajectory::dp(double t, int i) {
            glm::vec3 T = glm::vec3(pow(t,2), t, 1);
            glm::mat3x4 R = getRMatrix(i);

            return glm::normalize(T * dBscale * dB * R);
        }

        glm::vec3 Trajectory::ddp(double t, int i) {
            glm::vec2 T = glm::vec2(t,1);
            glm::mat3x4 R = getRMatrix(i);

            return glm::normalize(T * ddBscale * ddB * R);
        }

        void Trajectory::calculatePath() {
            for(int i = 1, n = rs.size() - 2; i < n; i++) {
                for(double t = 0; t < 1; t += 0.01)
                {
                    ps.push_back(p(t,i));
                }
            }
        }

        void Trajectory::draw() {
            glBegin(GL_LINE_STRIP);
            glColor3f(0.0,0.0,0.0);
            for(auto& p : ps) {
                glVertex3f(p.x,p.y,p.z);
            }
            glEnd();
        }
    }
}