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
        // 这种初始化不能存在 shape 为空以及 shape 太大的情况（必须小于等于 3）
        CHECK(!shapes.empty() && shapes.size() <= 3);
        // 剩余的维度
        uint32_t remaining = 3 - shapes.size();
        // 将 shapes_ 初始化为含有三个 1 元素的 vector
        std::vector<uint32_t> template_shapes(3, 1);
        // 将 shapes 里面的数据复制到 template_shapes 里面
        // 如果 shapes 是 2 维度的，那么，template_shapes 多出来的维度就被设置为 1
        std::copy(shapes.begin(), shapes.end(), template_shapes.begin());

        uint32_t channels = template_shapes.at(0);
        uint32_t rows = template_shapes.at(1);
        uint32_t cols = template_shapes.at(2);

        // 默认初始化
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
}

// 没有数据指针的构造函数
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
        // 这里传递的 shapes 的维度必须是 3 维的
        CHECK_EQ(shapes.size(), 3);
        // 这里不知道为什么不检测指针是不是空指针
        // CHECK_NE(raw_tensor_ptr, nullptr);

        const uint32_t channels = shapes.at(0);
        uint32_t rows = shapes.at(1);
        uint32_t cols = shapes.at(2);

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
        // 构造数据对象
        this->tensor_data_ = arma::Cube<T>(raw_tensor_ptr, rows, cols, channels, false, true);
    }
}
