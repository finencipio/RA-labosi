#include <iostream>
#include "mg/Vertex.h"
#include "mg/Triangle.h"
#include <GL/glut.h>
#include "mg/Object.h"
#include "animation/Trajectory.h"
#include "mg/Vertex.h"
#include <chrono>

GLuint window;
GLuint width = 700, height = 500;

void myDisplay();
void myIdle();
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);

std::chrono::high_resolution_clock::time_point currentTime;
std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();

int speed = 1;

double t = 0;
int i = 1;

ra::mg::Object *obj;
ra::animation::Trajectory *trajectory;

extern bool isDcm = false;

ra::mg::Vertex eye = {50, 40, 20};
ra::mg::Vertex center = {5, 5, 25};
ra::mg::Vertex up = {0, 0, 5};

int main(int argc, char** argv) {
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutInit(&argc, argv);

    window = glutCreateWindow("Animacija");
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);

    glMatrixMode(GL_PROJECTION);
//    glOrtho(-10, 10, -10, 10, 0.1, 100);
    glFrustum(-5,5,-5,5,10,100);
    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(myKeyboard);

    obj = new ra::mg::Object("747.obj");
    trajectory = new ra::animation::Trajectory("putanja.txt");

    glutIdleFunc(myIdle);
    glutMainLoop();
    return 0;
}

void myIdle()
{
    currentTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> delta = currentTime - previousTime;

    if(delta.count() > 10) {
        myDisplay();
        previousTime = currentTime;
    }
}

void myDisplay()
{

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(.32, .28, .28, 1);


    trajectory->draw();

    obj->draw(*trajectory, t,i);
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(5,0,0);
    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,5,0);
    glColor3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,5);
    glEnd();

    t += 0.005 * speed;
    if(t > 1) {
        t = 0;
        i = (i + 1);
        if(i > trajectory->segmentCount()) i = 1;
    } else if(t < 0) {
        t = 1;
        i = (i - 1);
        i = i < 1 ? trajectory->segmentCount() : i;
    }

    glFlush();
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
    switch(theKey) {
        case 'q':
            speed *= 2;
            break;
        case 'w':
            speed = (speed + 1) / 2;
            break;
        case 'i':
            eye.x += 1;
            break;
        case 'k':
            eye.x -= 1;
            break;
        case 'u':
            eye.y += 1;
            break;
        case 'j':
            eye.y -= 1;
            break;
        case 'z':
            eye.z += 1;
            break;
        case 'h':
            eye.z -= 1;
            break;
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
}