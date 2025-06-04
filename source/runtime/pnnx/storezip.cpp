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
        uint16_t flag;
        uint16_t compression;
        uint16_t last_modify_time;
        uint16_t last_modify_date;
        uint32_t crc32;
        uint32_t compressed_size;
        uint32_t uncompressed;
        uint16_t file_name_length;
        uint16_t extra_field_length;
        });

    PACK(struct zip64_extended_extra_field {
        uint64_t uncompressed_size;
        uint64_t compressed_size;
        uint64_t lfh_offset;
        uint32_t disk_number;
        });

    PACK(struct central_directory_file_header {
        uint16_t version_mode;
        uint16_t version;
        uint16_t flag;
        uint16_t compression;
        uint16_t last_modify_time;
        uint16_t last_modify_date;
        uint32_t crc32;
        uint32_t compressed_size;
        uint32_t uncompressed_size;
        uint16_t file_name_length;
        uint16_t extra_field_length;
        uint16_t file_comment_length;
        uint16_t start_disk;
        uint16_t internal_file_attrs;
        uint16_t external_file_attrs;
        uint32_t lfh_offset;
        });

    PACK(struct zip64_end_of_central_directory_record {
        uint64_t size_of_eocd64_m12;
        uint16_t version_made_by;
        uint16_t version_min_required;
        uint32_t disk_number;
        uint32_t start_disk;
        uint64_t cd_records;
        uint64_t total_cd_records;
        uint64_t cd_size;
        uint64_t cd_offset;
        });

    PACK(struct zip64_end_of_central_directory_locaator {
        uint32_t eocdr64_disk_number;
        uint64_t eocdr63_offset;
        uint32_t disk_count;
        });

    PACK(struct end_of_central_directory_record {
        uint16_t disk_number;
        uint16_t start_disk;
        uint16_t cd_records;
        uint16_t total_cd_records;
        uint32_t cd_size;
        uint32_t cd_offset;
        uint16_t comment_length;
        });
}

namespace pnnx {
    static uint32_t CRC32_TABLE[256];

    static void CRC32_TABLE_INIT() {
        for (int i = 0; i < 256; ++i) {
            uint32_t c = i;
            for (int j = 0; j < 8; ++j) {
                c & 1 ? c = (c >> 1) ^ 0xedb88320 : c >>= 1;
            }
            CRC32_TABLE[i] = c;
        }
    }

    static uint32_t CRC32(const uint32_t x, u_char ch) {
        return (x >> 8) ^ CRC32_TABLE[(x ^ ch) & 0xff];
    }

    static uint32_t CRC32_BUFFER(const u_char *data, const uint64_t length) {
        uint32_t x = 0xffffffff;
        for (uint64_t i = 0; i < length; ++i) {
            x = CRC32(x, data[i]);
        }
        return x ^ 0xffffffff;
    }
}
