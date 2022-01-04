#include "mg/Vertex.h"

namespace ra {
    namespace mg {
        std::ostream& operator<<(std::ostream &strm, const Vertex &v) {
            return strm << "x: " << v.x << ", y: " << v.y << ", z: " << v.z << '\n';
        }
    }
}