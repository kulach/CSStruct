#pragma once

#include <utest/utest.h>
#include <csstruct.h>
#include <stdint.h>

using namespace cssv;
struct IntegerStruct {
    int8_t i8 = 0;
    uint8_t u8 = 0;
    int16_t i16 = 0;
    uint16_t u16 = 0;
    int32_t i32 = 0;
    uint32_t u32 = 0;
    int64_t i64 = 0;
    uint64_t u64 = 0;

    static constexpr auto properties = std::make_tuple(
        property("i8", &IntegerStruct::i8),
        property("u8", &IntegerStruct::u8),
        property("i16", &IntegerStruct::i16),
        property("u16", &IntegerStruct::u16),
        property("i32", &IntegerStruct::i32),
        property("u32", &IntegerStruct::u32),
        property("i64", &IntegerStruct::i64),
        property("u64", &IntegerStruct::u64)
    );

};

struct FloatingStruct {
    float f = 0.0;
    double d = 0.0;
    long double ld = 0.0;

    static constexpr auto properties = std::make_tuple (
        property("f", &FloatingStruct::f),
        property("d", &FloatingStruct::d),
        property("ld", &FloatingStruct::ld)
    );
};
