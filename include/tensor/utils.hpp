//
// Created by aowei on 25-6-22.
//

#ifndef CINFER_TENSOR_UTILS_HPP
#define CINFER_TENSOR_UTILS_HPP
#include <tensor/tensor.hpp>

// 工具函数的声明
namespace cinfer
{
   template <typename T>
   sp_tensor<T> TensorPadding(const sp_tensor<T>& tensor, const std::vector<uint32_t>& pads, T padding_value);

   template <typename T>
   std::tuple<sp_tensor<T>, sp_tensor<T>> TensorBroadcast(const sp_tensor<T>& lhs, const sp_tensor<T>& rhs);

   template <typename T>
   bool TensorIsSame(const sp_tensor<T>& lhs, const sp_tensor<T>& rhs, T threshold = 1e-5f);

   template <typename T>
   sp_tensor<T> TensorElementAdd(const sp_tensor<T>& lhs, const sp_tensor<T>& rhs);

   template <typename T>
   void TensorElementAdd(const sp_tensor<T>& lhs, const sp_tensor<T>& rhs, const sp_tensor<T>& output);

   template <typename T>
   sp_tensor<T> TensorElementMultiply(const sp_tensor<T>& lhs, const sp_tensor<T>& rhs);

   template <typename T>
   void TensorElementMultiply(const sp_tensor<T>& lhs, const sp_tensor<T>& rhs, const sp_tensor<T>& output);

   template <typename T>
   sp_tensor<T> TensorCreate(uint32_t size);

   template <typename T>
   sp_tensor<T> TensorCreate(uint32_t rows, uint32_t cols);

   template <typename T>
   sp_tensor<T> TensorCreate(uint32_t channels, uint32_t rows, uint32_t cols);

   template <typename T>
   sp_tensor<T> TensorCreate(const std::vector<uint32_t>& shapes);

   template <typename T>
   sp_tensor<T> TensorClone(sp_tensor<T> tensor);
}

// 工具函数的实现
namespace cinfer
{
}
#endif //CINFER_TENSOR_UTILS_HPP
