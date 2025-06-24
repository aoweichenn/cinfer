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
        std::copy(shapes.begin(), shapes.end(), template_shapes.begin() + remaining);

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

// 获取某些信息的方法
namespace cinfer
{
    template <typename T>
    uint32_t Tensor<T>::rows() const
    {
        CHECK(!this->raw_tensor_shapes_.empty()) << "The data area of the tensor is empty.";
        // 返回数据的 row 的数量
        return this->tensor_data_.n_rows;
    }

    template <typename T>
    uint32_t Tensor<T>::cols() const
    {
        CHECK(!this->raw_tensor_shapes_.empty()) << "The data area of the tensor is empty.";
        // 返回数据的 col 的数量
        return this->tensor_data_.n_cols;
    }

    template <typename T>
    uint32_t Tensor<T>::channels() const
    {
        CHECK(!this->raw_tensor_shapes_.empty()) << "The data area of the tensor is empty.";
        // 返回数据的 channel 的数量
        return this->tensor_data_.n_slices;
    }

    template <typename T>
    size_t Tensor<T>::elements_size() const
    {
        CHECK(!this->raw_tensor_shapes_.empty()) << "The data area of the tensor is empty.";
        return this->tensor_data_.size();
    }

    // 返回一个横切面的大小，就是 row*col
    template <typename T>
    size_t Tensor<T>::planes_size() const
    {
        CHECK(!this->raw_tensor_shapes_.empty()) << "The data area of the tensor is empty.";
        return this->rows() * this->cols();
    }

    // 判断方式全都是以数据的纬度为准
    template <typename T>
    bool Tensor<T>::empty() const
    {
        return this->tensor_data_.empty();
    }

    template <typename T>
    T& Tensor<T>::index(uint32_t offset)
    {
        CHECK(offset < this->tensor_data_.size()) << "Tensor index out of bounds.";
        return this->tensor_data_.at(offset);
    }

    template <typename T>
    const T Tensor<T>::index(uint32_t offset) const
    {
        CHECK(offset < this->tensor_data_.size()) << "Tensor index out of bounds.";
        return this->tensor_data_.at(offset);
    }

    template <typename T>
    std::vector<T> Tensor<T>::shapes() const
    {
        CHECK(!this->tensor_data_.empty()) << "The data area of the tensor is empty.";
        return {this->channels(), this->rows(), this->cols()};
    }

    // 这里注意返回的是 shape 变量里面的 shape，不是从数据里面得到的 shape
    template <typename T>
    const std::vector<T>& Tensor<T>::raw_tensor_shapes() const
    {
        CHECK(!this->raw_tensor_shapes.empty());
        CHECK_LE(this->raw_tensor_shapes.size(), 3);
        CHECK_GE(this->raw_tensor_shapes.size(), 1);
        return this->raw_tensor_shapes_;
    }

    template <typename T>
    arma::Cube<T>& Tensor<T>::data() const
    {
        return this->tensor_data_;
    }

    template <typename T>
    const arma::Cube<T>& Tensor<T>::data() const
    {
        return this->tensor_data_;
    }

    template <typename T>
    arma::Mat<T>& Tensor<T>::slice(const uint32_t channel)
    {
        CHECK_LT(channel, this->channels());
        return this->tensor_data_.slice(channel);
    }

    template <typename T>
    const arma::Mat<T>& Tensor<T>::slice(uint32_t channel) const
    {
        CHECK_LT(channel, this->channels());
        return this->tensor_data_.slice(channel);
    }

    template <typename T>
    T& Tensor<T>::at(uint32_t channel, uint32_t row, uint32_t col)
    {
        CHECK_LT(row, this->rows());
        CHECK_LT(col, this->cols());
        CHECK_LT(channel, this->channels());
        return this->tensor_data_.at(row, col, channel);
    }

    template <typename T>
    const T Tensor<T>::at(uint32_t channel, uint32_t row, uint32_t col) const
    {
        CHECK_LT(row, this->rows());
        CHECK_LT(col, this->cols());
        CHECK_LT(channel, this->channels());
        return this->tensor_data_.at(row, col, channel);
    }

    template <typename T>
    T* Tensor<T>::raw_tensor_ptr()
    {
        // TODO: 详细看看 this->tensor_data_.memptr()
        CHECK(!this->tensor_data_.empty()) << "The data area of the tensor is empty.";
        return this->tensor_data_.memptr();
    }

    template <typename T>
    const T* Tensor<T>::raw_tensor_ptr() const
    {
        CHECK(!this->tensor_data_.empty()) << "The data area of the tensor is empty.";
        return this->tensor_data_.memptr();
    }

    template <typename T>
    T* Tensor<T>::raw_tensor_ptr(size_t offset)
    {
        CHECK(!this->tensor_data_.empty()) << "The data area of the tensor is empty.";
        const size_t size = this->elements_size();
        CHECK_LT(offset, size);
        return this->tensor_data_.memptr() + offset;
    }

    template <typename T>
    const T* Tensor<T>::raw_tensor_ptr(size_t offset) const
    {
        CHECK(!this->tensor_data_.empty()) << "The data area of the tensor is empty.";
        const size_t size = this->elements_size();
        CHECK_LT(offset, size);
        return this->tensor_data_.memptr() + offset;
    }

    // 就是返回按照 channel 分组的数据指针，比如 index = 1，就是第二个平面
    template <typename T>
    T* Tensor<T>::matrix_raw_tensor_ptr(uint32_t index)
    {
        CHECK_LT(index, this->channels());
        const size_t offset = index * this->planes_size();
        CHECK_LT(offset, this->elements_size());
        T* memptr = this->raw_tensor_ptr(offset);
        return memptr;
    }

    template <typename T>
    const T* Tensor<T>::matrix_raw_tensor_ptr(uint32_t index) const
    {
        CHECK_LT(index, this->channels());
        const size_t offset = index * this->planes_size();
        CHECK_LT(offset, this->elements_size());
        const T* memptr = this->raw_tensor_ptr(offset);
        return memptr;
    }

    // 按照 channel 输出数据
    template <typename T>
    void Tensor<T>::show()
    {
        for (uint32_t i = 0; i < this->channels(); ++i)
        {
            LOG(INFO) << "Channel: " << i;
            LOG(INFO) << "\n" << this->tensor_data_.slice(i);
        }
    }
}

// 修改数据的方法
namespace cinfer
{
    template <typename T>
    void Tensor<T>::set_data(const arma::Cube<T>& data)
    {
        // 维度一样才能赋值
        CHECK(data.n_rows == this->tensor_data_.n_rows) << data.n_rows << " != " << this->tensor_data_.n_rows;
        CHECK(data.n_cols == this->tensor_data_.n_cols) << data.n_cols << " != " << this->tensor_data_.n_cols;
        CHECK(data.n_slices == this->tensor_data_.n_slices) << data.n_slices << " != " << this->tensor_data_.n_slices;
        this->tensor_data_ = data;
    }
}
