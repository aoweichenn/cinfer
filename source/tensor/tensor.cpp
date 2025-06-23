//
// Created by aowei on 25-6-24.
//
#include <tensor/tensor.hpp>

// 没有数据指针的构造函数
namespace cinfer
{
    template <typename T>
    Tensor<T>::Tensor(uint32_t size)
    {
        // TODO: arma::Cube<T>(1, size, 1)函数详解
        // TODO: 补充注释
        this->tensor_data_ = arma::Cube<T>(1, size, 1);
        this->raw_tensor_shapes_ = std::vector<uint32_t>{size};
    }

    template <typename T>
    Tensor<T>::Tensor(uint32_t rows, uint32_t cols)
    {
        // TODO: 补充注释
        this->tensor_data_ = arma::Cube<T>(rows, cols, 1);
        if (rows == 1)
        {
            this->raw_tensor_shapes_ = std::vector<uint32_t>{cols};
        }
        else
        {
            this->raw_tensor_shapes_ = std::vector<uint32_t>{rows, cols};
        }
    }

    template <typename T>
    Tensor<T>::Tensor(uint32_t channels, uint32_t rows, uint32_t cols)
    {
        // TODO: 补充注释
        this->tensor_data_ = arma::Cube<T>(rows, cols, channels);
        if (channels == 1 && rows == 1)
        {
            this->raw_tensor_shapes_ = std::vector<uint32_t>{cols};
        }
        else if (channels == 1)
        {
            this->raw_tensor_shapes_ = std::vector<uint32_t>{rows, cols};
        }
        else
        {
            this->raw_tensor_shapes_ = std::vector<uint32_t>{channels, rows, cols};
        }
    }

    // TODO: 完善代码
    template <typename T>
    Tensor<T>::Tensor(const std::vector<uint32_t>& shapes)
    {
        CHECK(!shapes.empty() && shapes.size() <= 3);
    }
}

namespace cinfer
{
    // 构造一维数据
    template <typename T>
    Tensor<T>::Tensor(T* raw_tensor_ptr, uint32_t size)
    {
        CHECK_NE(raw_tensor_ptr, nullptr);
        // 一维数据
        this->raw_tensor_shapes_ = {size};
        // 给 tensro数据赋值
        // TODO:开一篇文档专门讲讲 arma 的参数和方法
        this->tensor_data_ = arma::Cube<T>(raw_tensor_ptr,
                                           1, size, 1, false, true);
    }

    // TODO: 完善注释
    template <typename T>
    Tensor<T>::Tensor(T* raw_tensor_ptr, uint32_t rows, uint32_t cols)
    {
        CHECK_NE(raw_tensor_ptr, nullptr);
        this->tensor_data_ = arma::Cube<T>(raw_tensor_ptr, rows, cols, 1, false, true);
        if (rows == 1)
        {
            this->raw_tensor_shapes_ = std::vector<uint32_t>{cols};
        }
        else
        {
            this->raw_tensor_shapes_ = std::vector<uint32_t>{rows, cols};
        }
    }

    // TODO: 完善注释
    template <typename T>
    Tensor<T>::Tensor(T* raw_tensor_ptr, uint32_t channels, uint32_t rows, uint32_t cols)
    {
        CHECK_NE(raw_tensor_ptr, nullptr);
        this->tensor_data_ = arma::Cube<T>(raw_tensor_ptr, rows, cols, channels, false, true);
        if (channels == 1 && rows == 1)
        {
            this->raw_tensor_shapes_ = std::vector<uint32_t>{cols};
        }
        else if (channels == 1)
        {
            this->raw_tensor_shapes_ = std::vector<uint32_t>{rows, cols};
        }
        else
        {
            this->raw_tensor_shapes_ = std::vector<uint32_t>{channels, rows, cols};
        }
    }

    // TODO: 完善注释，这里的逻辑比较特殊
    template <typename T>
    Tensor<T>::Tensor(T* raw_tensor_ptr, const std::vector<uint32_t>& shapes)
    {
        CHECK_EQ(shapes.size(), 3);
        CHECK_NE(raw_tensor_ptr, nullptr);
    }
}
