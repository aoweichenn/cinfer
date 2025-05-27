//
// Created by aoweichen on 2025/5/27.
//

#include <openblas/cblas.h>  // 或完整路径 #include "include/cblas.h"
#include <iostream>

int main() {
    double x[] = {1.0, 2.0, 3.0};
    double y[] = {4.0, 5.0, 6.0};
    double dot = cblas_ddot(3, x, 1, y, 1);
    std::cout << "Dot product: " << dot << std::endl;
    return 0;
}
