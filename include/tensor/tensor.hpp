//
// Created by aowei on 25-6-22.
//

#ifndef CINFER_TENSOR_TENSOR_HPP
#define CINFER_TENSOR_TENSOR_HPP

#include <glog/logging.h>
#include <armadillo>
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

    public:
        [[nodiscard]] uint32_t rows() const;
        [[nodiscard]] uint32_t cols() const;
        [[nodiscard]] uint32_t channels() const;
        [[nodiscard]] size_t elements_size() const;
        [[nodiscard]] size_t planes_size() const;
        [[nodiscard]] bool empty() const;
        T& index(uint32_t offset);
        const T& index(uint32_t offset) const;
        std::vector<T> shapes() const;
        const std::vector<T>& raw_tensor_shapes() const;
        arma::cube<T>& data() const;
        const arma::cube<T>& data() const;
        arma::Mat<T>& slice(uint32_t channel);
        const arma::Mat<T>& slice(uint32_t channel) const;
        T& at(uint32_t channel, uint32_t row, uint32_t col);
        const T at(uint32_t channel, uint32_t row, uint32_t col) const;
        T* raw_tensor_ptr();
        const T* raw_tensor_ptr() const;
        T* raw_tensor_ptr(size_t offset);
        const T* raw_tensor_ptr(size_t offset) const;
        T* matrix_raw_tensor_ptr(uint32_t index);
        const T* matrix_raw_tensor_ptr(uint32_t index) const;
        void show();

    public:
        void set_data(const arma::Cube<T>& data);
        void padding(const std::vector<uint32_t>& pads, T padding_value);
        void fill(T fill_value);
        void fill(const std::vector<T>& values, bool row_major = true);
        std::vector<T> values(bool row_major = true);
        void ones();
        void randn(T mean = 0, T var = 1);
        void randu(T min = 0, T max = 1);
        void reshape(const std::vector<uint32_t>& shapes, bool row_major = false);
        void flatten(bool row_major = false);
        // 对数据做一些变换
        void transform(const std::function<T(T)>& filter);

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
