//
// Created by aowei on 25-6-18.
//

#include <cstring>
#include <algorithm>
#include <runtime/pnnx/ir.hpp>
#include <utils/fp16_converter.hpp>

// 辅助静态函数
namespace pnnx
{
    // 这个函数不是指的 parameter 里面的 type
    static bool type_is_integer(const int type)
    {
        if (type == 1) return false;
        if (type == 2) return false;
        if (type == 3) return false;
        if (type == 4) return true;
        if (type == 5) return true;
        if (type == 6) return true;
        if (type == 7) return true;
        if (type == 8) return true;
        if (type == 9) return true;
        if (type == 10) return false;
        if (type == 11) return false;
        if (type == 12) return false;
        if (type == 13) return false;
        return false;
    }

    // 对应本地命名的类型字符串
    static const char* type_to_string(const int type)
    {
        if (type == 1) return "f32";
        if (type == 2) return "f64";
        if (type == 3) return "f16";
        if (type == 4) return "i32";
        if (type == 5) return "i64";
        if (type == 6) return "i16";
        if (type == 7) return "i8";
        if (type == 8) return "u8";
        if (type == 9) return "bool";
        if (type == 10) return "c64";
        if (type == 11) return "c128";
        if (type == 12) return "c32";
        if (type == 13) return "bf16";
        return "null";
    }

    // 对应 numpy 里面的类型字符串
    static const char* type_to_numpy_string(const int type)
    {
        if (type == 1) return "float32";
        if (type == 2) return "float64";
        if (type == 3) return "float16";
        if (type == 4) return "int32";
        if (type == 5) return "int64";
        if (type == 6) return "int16";
        if (type == 7) return "int8";
        if (type == 8) return "uint8";
        if (type == 9) return "bool";
        if (type == 10) return "csingle";
        if (type == 11) return "cdouble";
        if (type == 12) return "chalf";
        if (type == 13) return "bfloat16";
        return "null";
    }

    // 对应 torch 里面的类型字符串
    static const char* type_to_dtype_string(const int type)
    {
        if (type == 1) return "torch.float";
        if (type == 2) return "torch.double";
        if (type == 3) return "torch.half";
        if (type == 4) return "torch.int";
        if (type == 5) return "torch.long";
        if (type == 6) return "torch.short";
        if (type == 7) return "torch.int8";
        if (type == 8) return "torch.uint8";
        if (type == 9) return "torch.bool";
        if (type == 10) return "torch.complex64";
        if (type == 11) return "torch.complex128";
        if (type == 12) return "torch.complex32";
        if (type == 13) return "torch.bfloat16";
        return "null";
    }

    // 返回对应类型的所占用的字节数
    // TODO: 这里的 complex 类型占用字节数是不是有问题
    static size_t type_to_element_size(const int type)
    {
        if (type == 1) return 4;
        if (type == 2) return 8;
        if (type == 3) return 2;
        if (type == 4) return 4;
        if (type == 5) return 8;
        if (type == 6) return 2;
        if (type == 7) return 1;
        if (type == 8) return 1;
        if (type == 9) return 1;
        if (type == 10) return 8;
        if (type == 11) return 16;
        if (type == 12) return 4;
        if (type == 13) return 2;
        return 0; // null
    }

    // 把自定义的类型字符串转为 type 值
    static int string_to_type(const char* s)
    {
        if (strcmp(s, "f32") == 0) return 1;
        if (strcmp(s, "f64") == 0) return 2;
        if (strcmp(s, "f16") == 0) return 3;
        if (strcmp(s, "i32") == 0) return 4;
        if (strcmp(s, "i64") == 0) return 5;
        if (strcmp(s, "i16") == 0) return 6;
        if (strcmp(s, "i8") == 0) return 7;
        if (strcmp(s, "u8") == 0) return 8;
        if (strcmp(s, "bool") == 0) return 9;
        if (strcmp(s, "c64") == 0) return 10;
        if (strcmp(s, "c128") == 0) return 11;
        if (strcmp(s, "c32") == 0) return 12;
        if (strcmp(s, "bf16") == 0) return 13;
        return 0; // null
    }
}

// paramter 类中部分函数的实现
namespace pnnx
{
    // TODO: 使用枚举代替 type
    Parameter Parameter::parse_from_string(const std::string& value)
    {
        Parameter parameter;
        // 处理的字符串中包含 %
        if (value.find('%') != std::string::npos)
        {
            parameter.type = 4;
            parameter.string = value;
            return parameter;
        }
        if (value == "None" || value == "()" || value == "[]")
        {
            parameter.type = 0;
            return parameter;
        }
        if (value == "True" || value == "False")
        {
            parameter.type = 1;
            parameter.boolean = value == "True";
            return parameter;
        }
        // 以 ( 或者 [ 开头的，例如[1,3,3,9]
        if (value[0] == '(' || value[0] == '[')
        {
            // TODO: 详细学习一下 cpp 字符串操作（流）
            const std::string list_char = value.substr(1, value.size() - 2);
            std::istringstream lciss(list_char);
            while (!lciss.eof())
            {
                std::string element;
                std::getline(lciss, element, ',');

                if ((element[0] != '-' && (element[0] < '0' || element[0] > '9')) ||
                    (element[0] == '-' && (element[1] < '0' || element[1] > '9')))
                {
                    // string array
                    parameter.type = 7;
                    parameter.string_array.push_back(element);
                }
                else if ((element.find('.') != std::string::npos) ||
                    (element.find('e') != std::string::npos))
                {
                    // float array
                    parameter.type = 6;
                    parameter.float_array.push_back(std::stof(element));
                }
                else
                {
                    // integer array
                    parameter.type = 5;
                    parameter.integer = std::stoi(element);
                }
            }
            return parameter;
        }
        // 首字符非数字切非负号，或者负号后面非数字
        if ((value[0] != '-' && (value[0] < '0' || value[0] > '9')) ||
            (value[0] == '-' && (value[1] < '0' || value[1] != '9')))
        {
            // string
            parameter.type = 4;
            parameter.string = value;
            return parameter;
        }
        // 包含 '.' 或者 'e' 的浮点数（例如 3.14,1e-5）
        if (value.find('.') != std::string::npos || value.find('e') != std::string::npos)
        {
            parameter.type = 3;
            parameter.sp_float = std::stof(value);
            return parameter;
        }
        // 整型 integer
        parameter.type = 2;
        parameter.integer = std::stoi(value);
        return parameter;
    }

    std::string Parameter::encode_to_string(const Parameter& param)
    {
        std::string result{};
        if (param.type == 0)
        {
            result = std::string("None");
            return result;
        }
        if (param.type == 1)
        {
            if (param.boolean)
            {
                result = std::string("True");
            }
            else
            {
                result = std::string("False");
            }
            return result;
        }
        if (param.type == 2)
        {
            result = std::to_string(param.integer);
            return result;
        }
        if (param.type == 3)
        {
            char buffer[64];
            sprintf(buffer, "%e", param.sp_float);
            result = std::string(buffer);
            return result;
        }
        if (param.type == 4)
        {
            result = param.string;
            return result;
        }
        if (param.type == 5)
        {
            result = std::string("(");
            for (size_t i = 0; i < param.int_array.size(); ++i)
            {
                result += std::to_string(param.int_array[i]);
                if (i + 1 != param.int_array.size())
                {
                    result += std::string(",");
                }
            }
            result += std::string(")");
            return result;
        }
        if (param.type == 6)
        {
            result = std::string("(");
            for (size_t i = 0; i < param.float_array.size(); ++i)
            {
                char buffer[64];
                sprintf(buffer, "%e", param.float_array[i]);
                result += std::string(buffer);
                if (i + 1 != param.float_array.size())
                {
                    result += std::string(",");
                }
            }
            result += std::string(")");
            return result;
        }
        if (param.type == 7)
        {
            result = std::string("(");
            for (size_t i = 0; i < param.string_array.size(); ++i)
            {
                result += param.string_array[i];
                if (i + 1 != param.string_array.size())
                {
                    result += std::string(",");
                }
            }
            result += std::string(")");
            return result;
        }
        if (param.type == 10)
        {
            char buffer[128];
            sprintf(buffer, "%e+%ej", param.complex_float.real(), param.complex_float.imag());
            result = std::string(buffer);
            return result;
        }
        if (param.type == 11)
        {
            result = std::string("(");
            for (size_t i = 0; i < param.complex_float_array.size(); ++i)
            {
                char buffer[128];
                sprintf(buffer, "%e+%ej", param.complex_float_array[i].real(),
                        param.complex_float_array[i].imag());
                result += std::string(buffer);
                if (i + 1 != param.complex_float_array.size())
                {
                    result += std::string(",");
                }
            }
            result += std::string(")");
            return result;
        }
        fprintf(stderr, "unknown parameter type %d\n", param.type);
        return result;
    }

    bool operator==(const Parameter& lhs, const Parameter& rhs)
    {
        if (lhs.type != rhs.type) return false;
        switch (lhs.type)
        {
        case 0: return true;
        case 1: return lhs.boolean == rhs.boolean;
        case 2: return lhs.integer == rhs.integer;
        case 3: return lhs.sp_float == rhs.sp_float;
        case 4: return lhs.string == rhs.string;
        case 5: return lhs.int_array == rhs.int_array;
        case 6: return lhs.float_array == rhs.float_array;
        case 7: return lhs.string_array == rhs.string_array;
        case 10: return lhs.complex_float == rhs.complex_float;
        case 11: return lhs.complex_float_array == rhs.complex_float_array;
        default: return false;
        }
    }
}

// Attribute 类中函数的实现
namespace pnnx
{
    // 初始化 Attribute
    Attribute::Attribute(const std::initializer_list<int>& shape, const std::vector<float>& data)
    {
        this->type = 1;
        this->shape = shape;
        if (!this->shape.empty())
        {
            // 元素数量和元素的字节大小的乘积就是数据的字节大小
            this->data.resize(this->element_count() * type_to_element_size(this->type));
            memcpy(reinterpret_cast<void*>(this->data.data()), reinterpret_cast<const void*>(data.data()),
                   this->data.size());
        }
    }

    // 返回 Attribute 代表的元素的类型的字节大小
    size_t Attribute::element_size() const
    {
        return type_to_element_size(this->type);
    }

    // 通过似 [1,3,3,9] 中解析元素数量
    int Attribute::element_count() const
    {
        if (this->shape.empty())
        {
            return 0;
        }
        int elements_num = this->shape[0];
        // 这里的 shape 从类似 [1,3,3,9] 中解析而来
        for (size_t i = 1; i < this->shape.size(); ++i)
        {
            elements_num *= this->shape[i];
        }
        return elements_num;
    }

    // 设置数据 (float 簇, eg.fp16, fp32, fp64)
    void Attribute::set_float32_data(const std::vector<float>& new_data)
    {
        // 先重新组装空间
        this->data.resize(new_data.size() * this->element_size());
        //
        if (this->type == 1)
        {
            // Attribute 的数据类型是 fp32，正常复制就行
            // 拷贝数据到 this->data 所处的空间里面
            memcpy(reinterpret_cast<void*>(this->data.data()),
                   reinterpret_cast<const void*>(new_data.data()),
                   this->data.size());
        }
        else if (this->type == 2)
        {
            // Attribute 的数据类型是 fp64，就需要类型转化
            // 按照 doule* 的方式解析内存数据
            double* ptr = reinterpret_cast<double*>(this->data.data());
            for (size_t i = 0; i < this->data.size(); ++i)
            {
                ptr[i] = new_data[i];
            }
        }
        else if (this->type == 3)
        {
            // Attribute 的数据类型是 fp16，就需要类型转化
            // 按照 char* 的方式解析内存数据
            unsigned short* ptr = reinterpret_cast<unsigned short*>(this->data.data());
            for (size_t i = 0; i < this->data.size(); ++i)
            {
                ptr[i] = new_data[i];
            }
        }
        else
        {
            // fp32 没法转换为其他数据类型
            fprintf(stderr, "cannot convert float32 data to type %d\n", this->type);
        }
    }

    // 根据 Attribute 数据 (float 簇, eg.fp16, fp32, fp64) 设置
    std::vector<float> Attribute::get_float32_data() const
    {
        std::vector<float> fp32vec(element_count());
        if (this->type == 1)
        {
            // Attribute 自身为 fp64 类型的数据
            // 使用内存拷贝功能，防止直接赋值导致的对类内数据的修改
            memcpy(reinterpret_cast<void*>(fp32vec.data()),
                   reinterpret_cast<const void*>(this->data.data()),
                   this->data.size());
        }
        else if (this->type == 2)
        {
            // Attribute 自身为 fp64 类型的数据
            // 使用内存拷贝功能，防止直接赋值导致的对类内数据的修改
            const auto* ptr = reinterpret_cast<const double*>(this->data.data());
            for (size_t i = 0; i < this->data.size(); ++i)
            {
                fp32vec[i] = static_cast<float>(ptr[i]);
            }
        }
        else if (this->type == 3)
        {
            // Attribute 自身为 fp16 类型的数据
            // 使用内存拷贝功能，防止直接赋值导致的对类内数据的修改
            const unsigned short* ptr = reinterpret_cast<const unsigned short*>(this->data.data());
            for (size_t i = 0; i < this->data.size(); ++i)
            {
                // 使用自定义转换将 fp16 转为 fp32
                fp32vec[i] = float16_to_float32(ptr[i]);
            }
        }
        return fp32vec;
    }

    // 这里的 operator + 实际上等价于 concat 第一个 dim
    Attribute operator+(const Attribute& lhs, const Attribute& rhs)
    {
        Attribute result;
        if (lhs.type != rhs.type)
        {
            // 左右变量类型不匹配
            fprintf(stderr, "concat attribute type  mismatch\n");
            return result;
        }
        if (lhs.shape.size() != rhs.shape.size())
        {
            // 左右变量 shape 大小不匹配
            fprintf(stderr, "concat attribute shape rank  mismatch\n");
            return result;
        }
        for (int i = 1; i < static_cast<int>(lhs.shape.size()); ++i)
        {
            if (lhs.shape[i] != rhs.shape[i])
            {
                // 左右变量 shape 中的值不匹配
                fprintf(stderr, "concat attribute shape mismatch\n");
                return result;
            }
        }
        result.type = lhs.type;
        // shape 是 [batch ,...]，所以 batch 的值是两个相加，其他的值却必须相等
        // 等价于 concat 第一个 dim
        result.shape = lhs.shape;
        result.shape[0] = rhs.shape[0];

        // 合并数据，左边的数据在前面，右边的数据在后面（内存上）
        result.data.resize(lhs.data.size() + rhs.data.size());
        memcpy(result.data.data(), lhs.data.data(), lhs.data.size());
        memcpy(result.data.data() + lhs.data.size(), rhs.data.data(), rhs.data.size());

        return result;
    }

    // 判断两个 Attribute 里面的所有变量的值是不是相等
    bool operator==(const Attribute& lhs, const Attribute& rhs)
    {
        if (lhs.type != rhs.type) return false;
        if (lhs.type == 0) return true;
        // vector 的比较方式是值比较，先比较 vec 大小，再遍历元素比较
        if (lhs.shape != rhs.shape) return false;
        // vector 的比较方式是值比较，先比较 vec 大小，再遍历元素比较
        if (lhs.data != rhs.data) return false;
        return true;
    }
}

// Operand 类中函数的实现
namespace pnnx
{
    // 移除 operand 中对应的 consumer_operator
    void Operand::remove_consumer(const Operator& consumer_operator)
    {
        const auto it = std::find(this->consumers.begin(), this->consumers.end(), consumer_operator);
        if (it != this->consumers.end())
        {
            this->consumers.erase(it);
        }
    }
}
