//
// Created by aoweichen on 2025/5/27.
//

#include <runtime/cnnx/ir/graph.hpp>

#include "runtime/cnnx/utils/storezip.hpp"

using namespace cnnx;
using namespace std;

// int main(int argc, char* argv[])
// {
//     Graph graph;
//     graph.load("./resnet18.param",
//                "./resnet18.bin");
// }
int main(int argc, char* argv[])
{
    StoreZipReader szr;
    szr.open("./resnet18.bin");
}
