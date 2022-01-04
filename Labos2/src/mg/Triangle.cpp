#include "mg/Triangle.h"

namespace ra {
    namespace mg {
        std::ostream& operator<<(std::ostream &strm, const Triangle &t) {
            return strm << "T1: " << t.t1
                        << "\nT2: " << t.t2
                        << "\nT3: " << t.t3 << '\n';
        }
    }
}


