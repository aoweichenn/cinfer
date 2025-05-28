//
// Created by aoweichen on 2025/5/28.
//

#ifndef STOREZIP_HPP
#define STOREZIP_HPP
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace pnnx {
    struct StoreZipMetaOfReader {
        uint64_t offset;
        uint64_t size;
    };

    struct StoreZipMetaOfWriter {
        std::string name;
        uint64_t lfh_offset;
        uint32_t crc32;
        uint64_t size;
    };
}

namespace pnnx {
    class StoreZipReader {
    public:
        StoreZipReader();

        ~StoreZipReader();

    private:
        FILE *fp;
        std::map<std::string, StoreZipMetaOfReader> filemetas;
    };
}

namespace pnnx {
    class StoreZipWriter {
    public:
        StoreZipWriter();

        ~StoreZipWriter();

        int open(const std::string &path);

        int write_file(const std::string &name, const char *data, uint64_t size);

        int close();

    private:
        FILE *fp;
        std::vector<StoreZipMetaOfWriter> filemetas;
    };
}

#endif //STOREZIP_HPP
