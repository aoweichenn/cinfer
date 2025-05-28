//
// Created by aoweichen on 2025/5/28.
//

#ifndef STOREZIP_HPP
#define STOREZIP_HPP


namespace pnnx {
#ifdef _MSC_VER
#define PACK(__Declaration__) __pragma(pack(push, 1)) __Declaration__ __pragma(pack(pop))
#else
#define PACK(__Declaration__) __Declaration__ __attribute__((__packed__))
#endif
}




#endif //STOREZIP_HPP
