#include <glm/glm.hpp>
#include <mg/Object.h>
#include <mg/Vertex.h>
#include <sstream>
#include <GL/glut.h>

extern bool isDcm;

namespace ra {
    namespace mg {
        Object::Object(std::string filePath) {
            std::ifstream file(filePath);
            if(!file) {
                std::cerr << "Obj file could not be read, source: " << filePath << std::endl;
                exit(1);
            }
            std::string line;

            double minX = DBL_MAX, maxX = DBL_MIN;

            while(getline(file, line)) {
                //v
                if (!line.compare(0, 1, "v"))
                {
                    std::istringstream iss(line);
                    char c;
                    double x,y,z;
                    iss >> c >> x >> y >> z;
                    minX = x < minX ? x : minX;
                    maxX = x > maxX ? x : maxX;
                    glm::vec3 v = glm::vec3(x,y,z);

                    vertices.push_back(Vertex(v.x,v.y,v.z));
                }
                else if(!line.compare(0, 1, "f")) //f
                {
                    std::istringstream iss(line);
                    char t;
                    int index1, index2, index3;
                    iss >> t >> index1 >> index2 >> index3;
                    Vertex v1 = vertices[index1 - 1], v2 = vertices[index2 - 1], v3 = vertices[index3 - 1];
                    center += v1 + v2 + v3;

                    triangles.push_back(Triangle(v1, v2, v3));
                } else {
                    continue;
                }
            }
            double scaleF = 5 / (maxX - minX);
            scaleVec = glm::vec3(scaleF, scaleF, scaleF);

            for(auto& triangle : triangles) {
                triangle.t1 *= scaleVec;
                triangle.t2 *= scaleVec;
                triangle.t3 *= scaleVec;
            }

            center /= ( 3 * triangles.size());
        }

        void drawLine(float r, float g, float b, glm::vec3 position, glm::vec3 vector) {
            glColor3f(r,g,b);
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();

            glTranslatef(position.x, position.y, position.z);
            glBegin(GL_LINES);

            glVertex3f(0, 0, 0);
            glVertex3f(vector.x * 10, vector.y * 10, vector.z * 10);
            glEnd();

            glPopMatrix();
        }

        void Object::draw(animation::Trajectory &trajectory, double t, int segment) {
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();

            if(isDcm)
                drawWithDMCRotation(trajectory, t, segment);
            else
                drawWithAxisRotation(trajectory, t, segment);

            glPopMatrix();
        }

        void Object::drawWithAxisRotation(animation::Trajectory &trajectory, double t, int segment) {
            glm::vec3 position = trajectory.p(t, segment);
            glm::vec3 tangent = trajectory.dp(t, segment);

            glm::vec3 delta = position;

            glm::vec3 start = glm::vec3(0,0,1);
            glm::vec3 os = glm::normalize(glm::cross(start, tangent));
            double fi = acos(glm::dot(start, tangent));
            glTranslatef(delta.x, delta.y, delta.z);
            glRotatef(fi / (M_PI) * 180, os.x, os.y, os.z);


            glColor3f(0.95, 0.85, 0.81);
            glBegin(GL_TRIANGLES);
            for(auto& triangle : triangles) {
                glm::vec3 v1 = triangle.t1;
                glm::vec3 v2 = triangle.t2;
                glm::vec3 v3 = triangle.t3;

                glVertex3f(v1.x, v1.y, v1.z);
                glVertex3f(v2.x, v2.y, v2.z);
                glVertex3f(v3.x, v3.y, v3.z);
            }
            glEnd();
            drawLine(1.0, 0.0, 0.0, glm::vec3(0,0,0), glm::vec3(1, 0, 0));
            drawLine(0.0, 1.0, 0.0, glm::vec3(0,0,0), glm::vec3(0, 1, 0));
            drawLine(0.0, 0.0, 1.0, glm::vec3(0,0,0), glm::vec3(0, 0, 1));
        }

        void Object::drawWithDMCRotation(animation::Trajectory &trajectory, double t, int segment) {
            glm::vec3 position = trajectory.p(t, segment);
            //z
            glm::vec3 tangent = trajectory.dp(t, segment);
            //y
            glm::vec3 normal = glm::normalize(glm::cross(trajectory.ddp(t, segment), tangent));
            //x
            glm::vec3 binormal = glm::normalize(glm::cross(normal, tangent));

            glm::vec3 delta = position;

            //DCM
            //draw X
            drawLine(1.0, 0.0, 0.0, position, binormal);
            //draw Y
            drawLine(0.0, 1.0, 0.0, position, normal);
            //draw Z
            drawLine(0.0, 0.0, 1.0, position, tangent);
            glm::mat3 Ri = glm::mat3(binormal, normal, tangent);


            glColor3f(0.95, 0.85, 0.81);
            glBegin(GL_TRIANGLES);
            for(auto& triangle : triangles) {
                glm::vec3 v1 = Ri * triangle.t1 + delta;
                glm::vec3 v2 = Ri * triangle.t2 + delta;
                glm::vec3 v3 = Ri * triangle.t3 + delta;

                glVertex3f(v1.x, v1.y, v1.z);
                glVertex3f(v2.x, v2.y, v2.z);
                glVertex3f(v3.x, v3.y, v3.z);
            }
            glEnd();
        }

    }
}