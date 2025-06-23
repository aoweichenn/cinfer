//
// Created by aoweichen on 2025/5/27.
//

#include <iostream>

#include <runtime/cnnx/ir/graph.hpp>

#include "runtime/cnnx/utils/storezip.hpp"

using namespace cnnx;
using namespace std;

int main(int argc, char* argv[])
{
    Graph graph;
    graph.load("./resnet18.param","./resnet18.bin");
    // graph.parse("./resnet18.param");
    const int error = graph.save("./test.param", "./test.bin");
    std::cout << error << std::endl;
}

// int main(int argc, char* argv[])
// {
//     StoreZipReader szr;
//     szr.open("./resnet18.bin");
// }
