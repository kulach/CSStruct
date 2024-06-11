#pragma once 

#include <utest/utest.h>
#include <csstruct.h>
#include <test_structs.h>
#include <type_traits>
#include <test_util.h>

using namespace cssv;
UTEST(ReaderTests, TestSimpleIntegerStruct) {
    IntegerStruct s;
    CsvReader<IntegerStruct> reader;
    reader.open_file("test/test_data/int_struct.csv");

    for (uint32_t i = 0; i < 100; i++) {
        reader.get_next_line(s);

        EXPECT_EQ(s.i8,  1*static_cast<int8_t>(i));
        EXPECT_EQ(s.u8,  2*i);
        EXPECT_EQ(s.i16, 3*static_cast<int16_t>(i));
        EXPECT_EQ(s.u16, 4*i);
        EXPECT_EQ(s.i32, 5*static_cast<int32_t>(i));
        EXPECT_EQ(s.u32, 6*i);
        EXPECT_EQ(s.i64, 7*i); 
        EXPECT_EQ(s.u64, 8*i);
    }
}

UTEST(ReaderTests, TestSimpleFloatingStruct) {
    FloatingStruct s;
    CsvReader<FloatingStruct> reader;
    reader.open_file("test/test_data/float_struct.csv");

    for (uint32_t i = 0; i < 100; i++) {
        reader.get_next_line(s);

        double fi = static_cast<double>(i) * .01;

        EXPECT_TRUE(feq(s.f, fi));
        EXPECT_TRUE(feq(s.d, fi));
        EXPECT_TRUE(feq(s.ld, fi));
    }
}
