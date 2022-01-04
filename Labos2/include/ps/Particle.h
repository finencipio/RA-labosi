//
// Created by antesha on 05. 12. 2021..
//

#ifndef LABOS1_PARTICLE_H
#define LABOS1_PARTICLE_H

#include "mg/Vertex.h"
#include "Color.h"
#include <GL/glut.h>
#include <queue>
#include <memory>


namespace ra {
    double myrand() {
        return (double)rand() * 1.0 / RAND_MAX;
    }

    double myrandangle() {
        return myrand() * 2 * M_PI;
    }

    float ttl = 1;
    float acc = 1;

    glm::mat3x3 identity = {{1,0,0}, {0, 1, 0}, {0, 0, 1}};
    std::vector<std::pair<float, Color>> fireColorStops = {
            {0.0, {1, 1, 1}},
            {0.2, {1, 1, 0}},
            {.4, {240./255, 80./255, 20./255}},
            {0.6, {140./255, 10./255, 0}},
            {0.9, {140./255, 10./255, 0}},
            {1, {0.4, 0, 0}}
    };

    std::vector<std::pair<float, Color>> windColorStops = {
            {0.0, {1, 1, 1}},
            {1.0, {1, 1, 1}}
    };


    class Particle {
    public:
        glm::vec3 position;
        glm::vec3 acceleration = {0, 0, 0};

        std::vector<std::pair<float, Color>> colorStops;

        float radius = 0.1;

        float lifetime = 0.0;

        bool dead = false;

        Particle(const glm::vec3 &p, const std::vector<std::pair<float, Color>> &colorStops):
        position(p), colorStops(colorStops) {};

        virtual void update(float elapsed) = 0;

        virtual void render() = 0;

        virtual void updateAcceleration(glm::vec3 acc) {};

    protected:
        glm::vec3 calcColor() {
            std::pair<float, Color> end = colorStops[0];
            std::pair<float, Color> start = colorStops[0];
            for(auto& pair : colorStops) {
                start = end;
                end = pair;
                if(pair.first > lifetime) {
                    break;
                }
            }
            float q = end.first - start.first < 1e-6 ? 0 : (lifetime - start.first) / (end.first - start.first);
            return (1 - q) * start.second.params + q * end.second.params;
        }
    };

    class FireParticle : public Particle {
    public:
        glm::vec3 velocity;
        FireParticle(const glm::vec3 &p, const glm::vec3 &v, const glm::vec3 &acc) : Particle(p, fireColorStops), velocity(v) {
            acceleration = acc;
        }

        FireParticle(const glm::vec3 &p, const glm::vec3 &v, const glm::vec3 &acc, std::vector<std::pair<float, Color>> fireColorStops) : Particle(p, fireColorStops), velocity(v) {
            acceleration = acc;
        }

        void update(float elapsed) override {
            lifetime += elapsed / ttl;

            position += elapsed * velocity + acceleration * elapsed * elapsed *0.5f;
            velocity += elapsed * acceleration;

            if(lifetime > 1) {
                dead = true;
            }
        }

        void updateAcceleration(glm::vec3 acc) override {
            acceleration = acc + glm::vec3(0, 1, 0);
        }

        void render() override {
            glm::vec3 color = calcColor();
            glColor3f(color.r, color.g, color.b);
            glPushMatrix();
            glTranslatef(position.x, position.y, position.z);
            glutSolidSphere( radius, 5, 5);
            glPopMatrix();
        }
    };

    class WindParticle : public Particle {
        std::deque<glm::vec3> last3Positions;
        glm::vec3* bbMin;
        glm::vec3* bbMax;
        float lastUpdated = 0;
    public:
        glm::vec3* direction;
        float* force;
        glm::vec3 mydir;
        WindParticle(const glm::vec3 &p, glm::vec3* direction, float* force, glm::vec3* bbMin, glm::vec3* bbMax)
        : Particle(p, windColorStops), direction(direction), force(force), bbMin(bbMin), bbMax(bbMax) {
            last3Positions.push_back(p);
            last3Positions.push_back(p);
            last3Positions.push_back(p);
            last3Positions.push_back(p);
            last3Positions.push_back(p);
            last3Positions.push_back(p);
            last3Positions.push_back(p);
            last3Positions.push_back(p);
            last3Positions.push_back(p);
            last3Positions.push_back(p);
            last3Positions.push_back(p);
            last3Positions.push_back(p);

            mydir = *direction;
        }

        void update(float elapsed) override {
            lifetime += elapsed / ttl;
            if(lifetime - lastUpdated < 0.02) {
                return;
            }

            elapsed = lifetime - lastUpdated;
            lastUpdated = lifetime;

            mydir += glm::vec3((myrand() - .5f) / 10, (myrand() - .5f) / 10, (myrand() - .5f) / 10);
            glm::vec3 middle = mydir + *direction * 0.5f;
            position += elapsed * middle * *force;// + glm::vec3(0, acc * elapsed * elapsed / 2, 0);

            last3Positions.pop_front();
            last3Positions.push_back(position);

            if(lifetime > 1 || position.x < bbMin->x || position.x > bbMax->x || position.y < bbMin->y
            || position.y > bbMax->y || position.z < bbMin->z || position.z > bbMax->z) {
                dead = true;
            }
        }

        void render() override {
            glm::vec3 color = calcColor();
            glColor3f(color.r, color.g, color.b);
            glPushMatrix();
            glBegin(GL_LINE_STRIP);
            glTranslatef(position.x, position.y, position.z);
            for(auto &p : last3Positions) {
                glVertex3f(p.x, p.y, p.z);
            }
            glEnd();
            glPopMatrix();
        }
    };

    class ParticleSystem {
    protected:
        float timeFromLastCreation;
        std::vector<std::unique_ptr<Particle>> particles;
    public:
        float delta;

        ParticleSystem(float delta) : delta(delta) {}

        virtual void clock(float diff) {
            for(int i = particles.size() - 1; i >= 0; i--) {
                particles[i]->update(diff);
                if(particles[i]->dead) {
                    particles.erase(particles.begin() + i);
                }
            }

            timeFromLastCreation += diff;
            if(timeFromLastCreation >= delta) {
                timeFromLastCreation -= delta;
                particles.push_back(createParticle());
            }
        }

        void render() {
            for(auto& particle : particles) {
                particle->render();
            }
        }

        virtual std::unique_ptr<Particle> createParticle() = 0;
    };

    class FireParticleSystem : public ParticleSystem {

        double fireRadius;
        glm::vec3 fireCenter;
        glm::vec3 outsideAcceleration = {0,0,0};
        std::vector<std::pair<float, Color>> fireColorStops;

    public:
        FireParticleSystem(float delta, glm::vec3 fireCenter, double fireRadius, std::vector<std::pair<float, Color>> fireColorStops) :
        ParticleSystem(delta), fireCenter(fireCenter), fireRadius(fireRadius) {
            this->fireColorStops =fireColorStops;
        }

        std::unique_ptr<Particle> createParticle() override {
            double distanceFromCenter = myrand() * fireRadius;
            double angleInSpawn = myrandangle();

            double x = distanceFromCenter * cos(angleInSpawn) + fireCenter.x;
            double z = distanceFromCenter * sin(angleInSpawn) + fireCenter.z;

            double theta = (myrand() - 0.5) * M_PI;
            double fi = myrandangle();

            return std::make_unique<FireParticle>(glm::vec3(x, fireCenter.y, z),
                                                  glm::vec3(sin(theta)*sin(fi), cos(theta), sin(theta) * cos(fi)),
                                                  outsideAcceleration, fireColorStops);
        }

        virtual void changeAcceleration(glm::vec3 acc) {
            outsideAcceleration = acc;
            for(auto &p : particles) {
                p->updateAcceleration(acc);
            }
        };
    };

    class WindParticleSystem : public ParticleSystem {
        glm::vec3 bbMin;
        glm::vec3 bbMax;

    public:
        glm::vec3 direction = {1, 0, 0};
        float force = 2;

        WindParticleSystem(float delta, glm::vec3 bbMin, glm::vec3 bbMax) :
                ParticleSystem(delta), bbMin(bbMin), bbMax(bbMax) {
        }

        std::unique_ptr<Particle> createParticle() override {
            glm::vec3 position =
                    {bbMin.x + myrand() * (bbMax.x - bbMin.x),
                     bbMin.y + myrand() * (bbMax.y - bbMin.y),
                     bbMin.z + myrand() * (bbMax.z - bbMin.z)};


            return std::make_unique<WindParticle>(position, &direction, &force, &bbMin, &bbMax);
        }

        void changeDirection(double angleX, double angleY, double angleZ) {
            glm::mat3x3 rotX, rotY, rotZ;
            rotX = rotY = rotZ = identity;
            if(angleX != 0) {
                rotX = {
                        {1, 0, 0},
                        {0,cos(angleX),sin(angleX)},
                        {0,-sin(angleX),cos(angleX)}
                };
            }
            if(angleY != 0) {
                rotY = {
                        {cos(angleY), 0, -sin(angleY)},
                        {0, 1, 0},
                        {sin(angleY), 0,cos(angleY)}
                };
            }
            if(angleZ != 0) {
                rotZ = {
                        {cos(angleZ),sin(angleZ), 0},
                        {-sin(angleZ),cos(angleZ), 0},
                        {0, 0, 1}
                };
            }
            direction = glm::normalize(rotX * rotY * rotZ * direction);
        }
    };
}

#endif //LABOS1_PARTICLE_H
