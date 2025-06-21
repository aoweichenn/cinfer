//
// Created by aowei on 25-6-19.
//

#ifndef FP16_CONVERTER_HPP
#define FP16_CONVERTER_HPP

namespace cnnx
{
    unsigned short float32_to_float16(float fp32);

    float float16_to_float32(unsigned short fp16);
}
#endif //FP16_CONVERTER_HPP
