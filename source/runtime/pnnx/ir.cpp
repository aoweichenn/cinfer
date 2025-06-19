//
// Created by aowei on 25-6-18.
//

#include <runtime/pnnx/ir.hpp>

// paramter 类中部分函数的实现
namespace pnnx
{
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
                    // string
                    parameter.type = 7;
                    parameter.string_array.push_back(element);
                }
                else if ((element.find('.') != std::string::npos) ||
                    (element.find('e') != std::string::npos))
                {
                    // float
                    parameter.type = 6;
                    parameter.float_array.push_back(std::stof(element));
                }
                else
                {
                    // integer
                    parameter.type = 5;
                    parameter.integer = std::stoi(element);
                }
            }
        }
        return parameter;
    }

    std::string Parameter::encode_to_string(const Parameter& param)
    {
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
