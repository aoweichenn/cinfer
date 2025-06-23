//
// Created by aowei on 25-6-22.
//

#ifndef CINFER_TENSOR_TENSOR_HPP
#define CINFER_TENSOR_TENSOR_HPP

#include <glog/logging.h>
#include <armadillo>
#include <memory>
#include <vector>

namespace cinfer
{
    template <typename T>
    class Tensor
    {
    public:
        //
        explicit Tensor() = default;
        //
        explicit Tensor(uint32_t size);
        //
        explicit Tensor(uint32_t rows, uint32_t cols);
        //
        explicit Tensor(uint32_t channels, uint32_t rows, uint32_t cols);
        //
        explicit Tensor(const std::vector<uint32_t>& shapes);
        // 显式初始化一维 tensor，数据写在 arma 的 cols 里，数据来源于一个数据指针
        explicit Tensor(T* raw_tensor_ptr, uint32_t size);
        // 显式初始化二维 tensor，数据写在 arma 的 cols 里，数据来源于一个数据指针
        explicit Tensor(T* raw_tensor_ptr, uint32_t rows, uint32_t cols);
        // 显式初始化三维 tensor，数据写在 arma 的 cols 里，数据来源于一个数据指针
        explicit Tensor(T* raw_tensor_ptr, uint32_t channels, uint32_t rows, uint32_t cols);
        // 显式初始化指定 shapes 维 tensor，数据写在 arma 的 cols 里，数据来源于一个数据指针
        explicit Tensor(T* raw_tensor_ptr, const std::vector<uint32_t>& shapes);

    private:
        // TODO: 整改
        void review(const std::vector<uint32_t>& shapes);

    private:
        // 数据维度
        std::vector<uint32_t> raw_tensor_shapes_;
        // 数据
        arma::Cube<T> tensor_data_;
    };
}

#endif //CINFER_TENSOR_TENSOR_HPP
