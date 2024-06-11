#pragma once 

#include <utest/utest.h>
#include <csstruct.h>
#include <test_structs.h>
#include <type_traits>

using namespace cssv;
UTEST(ReaderTests, TestSimpleIntegerStruct) {
    IntegerStruct s;
    CsvReader<IntegerStruct> reader;
    reader.open_file("test/test_data/int_struct.csv");

    for (uint32_t i = 0; i < 100; i++) {
        reader.get_next_line(s);

        EXPECT_EQ(s.u8, i);
        EXPECT_EQ(s.i8, static_cast<int8_t>(i));
        EXPECT_EQ(s.u16, i);
        EXPECT_EQ(s.i16, static_cast<int16_t>(i));
        EXPECT_EQ(s.u32, i);
        EXPECT_EQ(s.i32, static_cast<int32_t>(i));
        EXPECT_EQ(s.u64, i);
        EXPECT_EQ(s.i64, i); 
    }
}
