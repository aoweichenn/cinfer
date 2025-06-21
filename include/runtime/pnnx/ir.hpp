//
// Created by aoweichen on 2025/5/28.
//

#ifndef PNNX_IR_HPP
#define PNNX_IR_HPP

#include <climits>
#include <complex>
#include <map>
#include <utility>
#include <vector>
// TODO:分析下面的代码具体做了什么
#if BUILD_TORCH2PNNX
namespace torch
{
    namespace jit
    {
        struct Value;
        struct Node;
    }
}
namespace at
{
    class Tensor;
}
namespace pnnx
{
    class TorchTensorProxy;
}
#endif

// TODO:分析下面的代码具体做了什么
#if BUILD_ONNX2PNNX
namespace onnx
{
    class AttributeProto;
    class TensorProto;
    class ValueInfoProto;
}
namespace pnnx::onnx2pnnx
{
    class OnnxAttributeProxy;
}
#endif

// parameter 类声明
namespace pnnx
{
    class Parameter
    {
    public:
        // TODO:优化类成员初始化
        // type 0 null
        Parameter(): type(0)
        {
        }

        // type 1 bool
        explicit Parameter(const bool _bool): type(1), boolean(_bool)
        {
        }

        // type 2 int
        // 对于 int,long,long long 数据类型，将他们统一转为内部的 int
        explicit Parameter(const int _int): type(2), integer(_int)
        {
        }

        explicit Parameter(long _long): type(2)
        {
            if (_long == std::numeric_limits<long>::max()) _long = INT_MAX;
            if (_long == std::numeric_limits<long>::min()) _long = INT_MIN;
            this->integer = static_cast<int>(_long);
        }

        explicit Parameter(long long _longlong): type(1)
        {
            if (_longlong == std::numeric_limits<long>::max()) _longlong = INT_MAX;
            if (_longlong == std::numeric_limits<long>::min()) _longlong = INT_MIN;
            this->integer = static_cast<int>(_longlong);
        }

        //type 3 float
        // 对于 float,double则统一转化为内部的float
        explicit Parameter(const float _float): type(3), sp_float(_float)
        {
        }

        explicit Parameter(const double _double): type(3), sp_float(static_cast<float>(_double))
        {
        }

        // type 4 string
        explicit Parameter(const char* _string): type(4), string(_string)
        {
        }

        explicit Parameter(std::string _string): type(4), string(std::move(_string))
        {
        }

        // type 5 int array(vector)
        Parameter(const std::initializer_list<int>& _int_array): type(5), int_array(_int_array)
        {
        }

        explicit Parameter(const std::vector<int>& _int_array): type(5), int_array(_int_array)
        {
        }

        Parameter(const std::initializer_list<int64_t>& _int_array): type(5)
        {
            for (const auto& _int : _int_array)
            {
                int64_t _long = _int;
                if (_long == std::numeric_limits<int64_t>::max()) _long = INT_MAX;
                if (_long == std::numeric_limits<int64_t>::min()) _long = INT_MIN;
                this->int_array.push_back(static_cast<int>(_long));
            }
        }

        explicit Parameter(const std::vector<int64_t>& _int_array): type(5)
        {
            for (const auto& _int : _int_array)
            {
                int64_t _long = _int;
                if (_long == std::numeric_limits<int64_t>::max()) _long = INT_MAX;
                if (_long == std::numeric_limits<int64_t>::min()) _long = INT_MIN;
                this->int_array.push_back(static_cast<int>(_long));
            }
        }

        //type 6 float array
        Parameter(const std::initializer_list<float>& _float_array): type(6), float_array(_float_array)
        {
        }

        explicit Parameter(const std::vector<float>& _float_array): type(6), float_array(_float_array)
        {
        }

        Parameter(const std::initializer_list<double>& _double_array): type(6)
        {
            for (const auto& _double : _double_array)
            {
                this->float_array.push_back(static_cast<float>(_double));
            }
        }

        explicit Parameter(const std::vector<double>& _double_array): type(6)
        {
            for (const auto& _double : _double_array)
            {
                this->float_array.push_back(static_cast<float>(_double));
            }
        }

        //type 7 string array
        Parameter(const std::initializer_list<const char*>& _string_array): type(7)
        {
            for (const auto& _string : _string_array)
            {
                this->string_array.emplace_back(_string);
            }
        }

        Parameter(const std::initializer_list<std::string>& _string_array): type(7), string_array(_string_array)
        {
        }

        explicit Parameter(const std::vector<std::string>& _string_array): type(7), string_array(_string_array)
        {
        }

        //type 10 complex float
        explicit Parameter(const std::complex<float>& _complex_float): type(10), complex_float(_complex_float)
        {
        }

        explicit Parameter(const std::complex<double>& _complex_double): type(10), complex_float(_complex_double)
        {
        }

        //type 11 complex float array
        Parameter(const std::initializer_list<std::complex<float>>& _complex_float_array): type(11),
            complex_float_array(_complex_float_array)
        {
        }

        explicit Parameter(const std::vector<std::complex<float>>& _complex_float_array): type(11),
            complex_float_array(_complex_float_array)
        {
        }

        Parameter(const std::initializer_list<std::complex<double>>& _complex_double_array): type(11)
        {
            for (const auto& _complex_double : _complex_double_array)
            {
                this->complex_float_array.push_back(static_cast<std::complex<float>>(_complex_double));
            }
        }

        explicit Parameter(const std::vector<std::complex<double>>& _complex_float_array): type(11)
        {
            for (const auto& _complex_float : _complex_float_array)
            {
                this->complex_float_array.push_back(static_cast<std::complex<float>>(_complex_float));
            }
        }

    public:
        static Parameter parse_from_string(const std::string& value);
        static std::string encode_to_string(const Parameter& param);

    public:
        // TODO:优化类成员初始化
        int type;
        bool boolean{};
        int integer{};
        float sp_float{};
        std::complex<float> complex_float{};
        std::vector<int> int_array{};
        std::vector<float> float_array{};
        std::vector<std::complex<float>> complex_float_array{};
        //TODO: 解析 keep std::string typed member the last for cross cxxabi compatibility
        std::string string{};
        std::vector<std::string> string_array{};
    };

    bool operator==(const Parameter& lhs, const Parameter& rhs);
}

// Attribute 类声明
namespace pnnx
{
    class Attribute
    {
        // type => {
        //  1 => fp32, 2 => fp64, 3 => fp16
        //  4 => i32, 5 => i64, 6 => i16, 7 => i8, 8 => u8
        //  9 => bool
        //  10 => c64, 11 => c128, 12 => c32, 13 => bf16
        // }
    public:
        Attribute() = default;

        Attribute(const std::initializer_list<int>& shape, const std::vector<float>& data);

        [[nodiscard]] size_t element_size() const;
        [[nodiscard]] int element_count() const;
        [[nodiscard]] std::vector<float> get_float32_data() const;
        void set_float32_data(const std::vector<float>& new_data);

    public:
        // type => {
        //  1 => fp32, 2 => fp64, 3 => fp16
        //  4 => i32, 5 => i64, 6 => i16, 7 => i8, 8 => u8
        //  9 => bool
        //  10 => c64, 11 => c128, 12 => c32, 13 => bf16
        // }
        int type{};
        std::vector<int> shape{};
        std::vector<char> data{};
        std::map<std::string, Parameter> params{};
    };

    bool operator==(const Attribute& lhs, const Attribute& rhs);

    Attribute operator+(const Attribute& lhs, const Attribute& rhs);
}

#endif //PNNX_IR_HPP
