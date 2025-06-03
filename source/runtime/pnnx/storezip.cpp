//
// Created by aoweichen on 2025/5/29.
//

#include <runtime/pnnx/storezip.hpp>

namespace pnnx {
#ifdef _MSC_VER
#define PACK(__Declaration__) __pragma(pack(push, 1)) __Declaration__ __pragma(pack(pop))
#else
#define PACK(__Declaration__) __Declaration__ __attribute__((__packed__))
#endif
}

namespace pnnx {
    PACK(struct local_file_header {
        uint16_t version;

        });
}
