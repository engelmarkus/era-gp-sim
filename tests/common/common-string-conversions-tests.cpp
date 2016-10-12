/*
* C++ Assembler Interpreter
* Copyright (C) 2016 Chair of Computer Architecture
* at Technical University of Munich
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "common/string-conversions.hpp"

TEST(StringConversionsTests, TestToHexStringConversion) {
  // Check if converting simple memory value works.
  EXPECT_EQ(StringConversions::toHexString(MemoryValue({0, 0}, 16)), "0x0000");

  // Check if converting simple memory value works.
  EXPECT_EQ(StringConversions::toHexString(MemoryValue({255, 200, 100, 2}, 32)), "0x0264c8ff");

  // Check if converting memory value work if its size is not a power of 2.
  EXPECT_EQ(StringConversions::toHexString(MemoryValue({0, 2, 2}, 20)), "0x20200");
}

TEST(StringConversionsTests, TestToBinStringConversion) {
  // Check if converting simple memory value works.
  EXPECT_EQ(StringConversions::toBinString(MemoryValue({0, 0}, 16)),
            "0000000000000000");

  // Check if converting simple memory value works.
  EXPECT_EQ(StringConversions::toBinString(MemoryValue({254, 2}, 16)),
            "0000001011111110");

  // Check if converting memory value works if its size is uneven.
  EXPECT_EQ(StringConversions::toBinString(MemoryValue({42, 1}, 9)), "100101010");
}

TEST(StringConversionsTests, TestHexStringToMemoryValue) {
  // Check if parsing simple string works.
  EXPECT_EQ(StringConversions::hexStringToMemoryValue("0000", 16),
            MemoryValue({0, 0}, 16));

  // Check if parsing simple string works.
  EXPECT_EQ(StringConversions::hexStringToMemoryValue("43ab89cd", 32),
            MemoryValue({205, 137, 171, 67}, 32));

  // Check if leading digits that exceed the specified memory value size are
  // automatically ignored.
  EXPECT_EQ(StringConversions::hexStringToMemoryValue("543ab89cd", 32),
            MemoryValue({205, 137, 171, 67}, 32));

  // Check if parsing string of memory value works if memory value size is not a
  // multiple of 4.
  EXPECT_EQ(StringConversions::hexStringToMemoryValue("1001", 14),
            MemoryValue({1, 16}, 14));

  // Check if "0x"-prefix works.
  EXPECT_EQ(StringConversions::hexStringToMemoryValue("0x543ab89cd", 32),
            MemoryValue({205, 137, 171, 67}, 32));

  // Check if parsing a string works, if it exceeds the memory value's specified length.
  EXPECT_EQ(StringConversions::hexStringToMemoryValue("5543ab89cd", 32),
            MemoryValue({205, 137, 171, 67}, 32));

  // Check if parsing a string works, if it misses the memory value's specified length.
  EXPECT_EQ(StringConversions::hexStringToMemoryValue("cd", 32),
            MemoryValue({205, 0, 0, 0}, 32));
}

TEST(StringConversionsTests, TestBinStringToMemoryValue) {
  // Check if parsing simple string works.
  EXPECT_EQ(StringConversions::binStringToMemoryValue("0000000000000000", 16),
            MemoryValue({0, 0}, 16));

  // Check if parsing simple string works.
  EXPECT_EQ(StringConversions::binStringToMemoryValue("0000001011111110", 16),
            MemoryValue({254, 2}, 16));

  // Check if parsing a string works, if the memory value's size is uneven.
  EXPECT_EQ(StringConversions::binStringToMemoryValue("100101010", 9),
            MemoryValue({42, 1}, 9));

  // Check if parsing a string works, if it exceeds the memory value's specified length.
  EXPECT_EQ(StringConversions::binStringToMemoryValue("100101010", 8),
            MemoryValue({42}, 8));

  // Check if parsing a string works, if it misses the memory value's specified length.
  EXPECT_EQ(StringConversions::binStringToMemoryValue("101010", 32),
            MemoryValue({42, 0, 0 ,0}, 32));
}
