#ifndef RA_1_TRIANGLE_H
#define RA_1_TRIANGLE_H

#include "Vertex.h"
#include <iostream>

namespace ra {
    namespace mg {

        class Triangle;

        std::ostream& operator<<(std::ostream& strm, const Triangle& t);

        class Triangle {
        private:
            friend std::ostream& operator<<(std::ostream& strm, const Triangle& t);
        public:
            Vertex t1,t2,t3;
            Triangle(Vertex t1, Vertex t2, Vertex t3):t1(t1), t2(t2), t3(t3) {}
        };
    }
}

#endif //RA_1_TRIANGLE_H
