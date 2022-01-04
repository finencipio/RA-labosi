//
// Created by antesha on 05. 12. 2021..
//

#ifndef LABOS1_COLOR_H
#define LABOS1_COLOR_H

namespace ra {
    class Color {
    public:
        float r, g, b, a;
        glm::vec3 params;

        Color(float r, float g, float b) : params(r,g,b) {}
        Color(glm::vec3 params) : params(params) {}
        Color(Color const &other) : params(other.params) {}
        Color() : params(0,0,0) {}

        Color operator+(Color other) {
            return {params + other.params};
        }

        void operator+=(Color other) {
            params += other.params;
        }

        void interpolate(float q, Color other) {
            params = params * q + other.params * (1 - q);
        }
    };
}

#endif //LABOS1_COLOR_H
