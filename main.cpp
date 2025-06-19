//
// Created by aoweichen on 2025/5/27.
//

#include <cstdint>
#include <runtime/pnnx/storezip.hpp>
using namespace pnnx;
using namespace std;

int main(int argc, char* argv[])
{
    uint64_t len = 1 * 1024 * 1024;
    char* data_1m = new char[len];
    StoreZipWriter szw;
    szw.open("szw.zip");
    szw.write_file("a.py", data_1m, len);
    szw.write_file("a.py", data_1m, len);
    szw.close();
    delete[] data_1m;

    StoreZipReader sz;

    sz.open("szw.zip");

    std::vector<std::string> names = sz.get_names();

    for (const auto& name : names)
    {
        const uint64_t size = sz.get_file_size(name);

        fprintf(stdout, "%s  %lu\n", name.c_str(), size);
    }

    sz.close();
    return 0;
}
