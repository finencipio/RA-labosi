#include <iostream>
#include "mg/Vertex.h"
#include "mg/Triangle.h"
#include <GL/glut.h>
#include "mg/Object.h"
#include "animation/Trajectory.h"
#include "mg/Vertex.h"
#include <chrono>
#include "ps/Particle.h"
#include "math.h"
#include "algorithm"

GLuint window;
GLuint width = 700, height = 500;

using namespace ra;

void myDisplay();

void myIdle();
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);
void updatePerspective();
void myReshape(int w, int h);

glm::vec3 O(-8.0, 4.7, -4.2);
glm::vec3 G(0, 0, 0);
glm::vec3 upvec(0, 1, 0);

std::vector<std::pair<float, Color>> fireColorStops2 = {
        {0.0, {1, 1, 1}},
        {0.2, {0, 1, 1}},
        {1, {0, 0, 0.4}}
};

FireParticleSystem fireParticles(0.005, {0,0,0}, 0.2, ra::fireColorStops);
FireParticleSystem fireParticles2(0.005, {02,0,02}, 0.2, fireColorStops2);
WindParticleSystem windParticles(0.020, {-8, 0, -8}, {8, 4, 8});

int main(int argc, char** argv) {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutInit(&argc, argv);

    window = glutCreateWindow("PS");

    fireParticles.changeAcceleration(windParticles.direction * windParticles.force);
    fireParticles2.changeAcceleration(windParticles.direction * windParticles.force);


    glMatrixMode(GL_PROJECTION);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    glutIdleFunc(myIdle);
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(myKeyboard);
    glutMainLoop();
    return 0;
}

int current_time;
int previous_time = 0;

void myIdle()
{
    current_time = glutGet(GLUT_ELAPSED_TIME);
    float diff = (current_time - previous_time) * 1.0 / 1000;

    if(diff > 0.005) {
        fireParticles.clock(diff);
        fireParticles2.clock(diff);
        windParticles.clock(diff);

        myDisplay();
        previous_time = current_time;
    }
}
void myReshape(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, width, height);
    updatePerspective();
}

void updatePerspective()
{
    glMatrixMode(GL_PROJECTION); // aktivirana matrica projekcije
    glLoadIdentity();
    gluPerspective(45.0, (float)width / height, 0.5, 80.0); // kut pogleda, x/y, prednja i straznja ravnina odsjecanja
    glMatrixMode(GL_MODELVIEW);                             // aktivirana matrica modela
    glLoadIdentity();
    gluLookAt(O.x, O.y, O.z, G.x, G.y, G.z, upvec.x, upvec.y, upvec.z); // O x,y,z; glediste x,y,z; up vektor x,y,z
}

void drawAxes() {
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(1,0,0);
    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,1,0);

    glColor3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,1);

    glEnd();
}

void myDisplay()
{

    glClear(GL_COLOR_BUFFER_BIT + GL_DEPTH_BUFFER_BIT);
    glClearColor(.32, .28, .28, 1);

    drawAxes();
    fireParticles.render();
    fireParticles2.render();
    windParticles.render();

    glutSwapBuffers();
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
    glm::vec3 zdir = glm::normalize((G - O));
    glm::vec3 xdir = glm::cross(zdir, upvec);
    switch(theKey) {
        case 'w':
            O += zdir;
            break;
        case 's':
            O -= zdir;
            break;
        case 'a':
            O -= xdir;
            break;
        case 'd':
            O += xdir;
            break;
        case ' ':
            G += xdir * (mouseX > width / 2 ? .1f : -.1f) + upvec * (mouseY > height / 2 ? -.1f : .1f);
        case 'l':
            fireParticles.delta = std::max(fireParticles.delta / 2.0, 0.001);
            break;
        case 'o':
            fireParticles.delta = std::min(fireParticles.delta * 2.0, 1.0);
            break;
        case 'x':
            windParticles.changeDirection(M_PI / 10, 0, 0);
            break;
        case 'y':
            windParticles.changeDirection(0, M_PI / 10, 0);
            break;
        case 'z':
            windParticles.changeDirection(0, 0, M_PI / 10);
            break;
        case 'f':
            windParticles.force += 0.5;
            break;
        case 'g':
            windParticles.force = std::max(0.5, windParticles.force - 0.5);
            break;
    }

    fireParticles.changeAcceleration(windParticles.direction * windParticles.force);

    updatePerspective();
}