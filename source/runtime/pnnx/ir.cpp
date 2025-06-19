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
        if (value[0] == '(' || value[0] == '[')
        {
            std::string list_char = value.substr(1, value.size() - 2);
        }
    }

    std::string Parameter::encode_to_string(const Parameter& param)
    {
    }

    bool operator==(const Parameter& lhs, const Parameter& rhs)
    {
    }
}
