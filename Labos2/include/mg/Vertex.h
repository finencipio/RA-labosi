#ifndef RA_1_VERTEX_H
#define RA_1_VERTEX_H

#include <glm/glm.hpp>
#include <iostream>


namespace ra {
    namespace mg {
        using Vertex = glm::vec3;

        std::ostream& operator<<(std::ostream &strm, const Vertex &v);
    }
}
#endif //RA_1_VERTEX_H
