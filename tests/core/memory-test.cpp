/* C++ Assembler Interpreter
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

#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

// Gtest has to be included before memory-value.
// clang-format off
#include "gtest/gtest.h"
#include "core/memory-value.hpp"
#include "core/memory.hpp"
#include "core/conversions.hpp"
// clang-format on

namespace {
constexpr std::size_t scale = 10;
}

TEST(memory, readWrite) {
  constexpr std::size_t b = 256;  // byteSize
  constexpr std::size_t c = 256;  // byteCount
  constexpr std::size_t t = scale;// testAmount
  std::uniform_int_distribution<std::uint16_t> dist{0, 255};
  std::mt19937 rand(0);// I need new numbers, I'm kinda really out of ideas
  std::size_t byteCount{1};
  std::size_t inc{0};
  while (byteCount <= c) {
    std::uniform_int_distribution<std::size_t> addressDist{0, byteCount - 1};
    for (std::size_t i = 1; i < b; ++i) {
      std::size_t byteSizeInByte{(i + 7) / 8};
      for (std::size_t j = 0; j < t; ++j) {
        const std::size_t rep{(byteCount + 9) / 10};
        Memory instance{byteCount, i};
        for (std::size_t l = 0; l < rep; ++l) {
          std::vector<std::uint8_t> initializer{};
          for (std::size_t k = 0; k < byteSizeInByte; ++k) {
            initializer.push_back(static_cast<std::uint8_t>(dist(rand)));
          }
          MemoryValue instance0{initializer, i};
          std::size_t address{addressDist(rand)};
          MemoryValue previous{instance.get(address)};
          MemoryValue instance1{instance.set(address, instance0)};
          ASSERT_EQ(instance0, instance.get(address));
          ASSERT_EQ(previous, instance1);
        }
      }
    }
    byteCount += ++inc;
  }
}

TEST(memory, serialization) {
  constexpr std::size_t memorySize = 1024 * 64 - 1;
  Memory instance0{memorySize, 8};
  std::uniform_int_distribution<std::uint16_t> dist{0, 255};
  std::mt19937 rand(0);// I need new numbers, I'm kinda really out of ideas
  for (std::size_t i = 0; i < memorySize; ++i) {
    instance0.put(
        i,
        conversions::convert(
            dist(rand), conversions::standardConversions::nonsigned, 8));
  }
  nlohmann::json json0{};
  instance0.serializeJSON(json0, ',', 64);
  Memory instance1{memorySize, 8};
  instance1.deserializeJSON(json0);
  ASSERT_EQ(instance0, instance1);
  nlohmann::json json1 = instance0.serializeJSON(nlohmann::json{}, ';', 1);
  Memory instance2{memorySize, 8};
  instance2.deserializeJSON(json1);
  ASSERT_EQ(instance0, instance2);
  Memory instance3{memorySize, 8};
  instance3.put(0, MemoryValue{std::vector<std::uint8_t>{1}, 8});
  instance3.put(memorySize - 1, MemoryValue{std::vector<std::uint8_t>{1}, 8});
  nlohmann::json json2 = instance3.serializeJSON(nlohmann::json{}, ';', 1);
  Memory instance4{json2};
  ASSERT_EQ(instance3, instance4);
}

TEST(memory, protection) {
  const MemoryValue FF{std::vector<std::uint8_t>{0xFF}, 8};
  const MemoryValue OO{std::vector<std::uint8_t>{0x00}, 8};
  const MemoryValue FFFF{std::vector<std::uint8_t>{0xFF, 0xFF}, 16};
  const MemoryValue OOOO{std::vector<std::uint8_t>{0x00, 0x00}, 16};
  const std::vector<std::size_t> protectedSet{2, 3, 6, 7, 10, 11};
  const std::vector<std::size_t> unprotectedSet{0, 1, 4, 5, 8, 9, 12, 13};
  // create memory with protection
  Memory memory(14, 8);// OOXXOOXXOO
  memory.makeProtected(2);
  memory.makeProtected(3);
  memory.makeProtected(7);
  memory.makeProtected(6);
  memory.makeProtected(10, 2);
  // Function to test
  std::function<bool(bool)> testFunc = [&](bool b) {
    for (auto i : protectedSet) {
      if (memory.get(i) != OO) return false;
    }
    for (auto i : unprotectedSet) {
      if (memory.get(i) != (b ? FF : OO)) return false;
    }
    return true;
  };
  // write into all cells
  for (std::size_t i = 0; i < 14; ++i) {
    memory.put(i, FF, false);
  }
  ASSERT_TRUE(testFunc(true));
  for (std::size_t i = 0; i < 14; ++i) {
    memory.put(i, OO, true);
  }
  for (std::size_t i = 1; i < 13; i += 2) {
    memory.put(i, FFFF, false);
  }
  ASSERT_TRUE(testFunc(false));
  for (std::size_t i = 0; i < 14; i += 2) {
    memory.put(i, FFFF, false);
  }
  // remove protection again
  ASSERT_TRUE(testFunc(true));
  for (auto i : protectedSet) {
    memory.removeProtection(i);
  }
  for (std::size_t i = 0; i < 14; ++i) {
    memory.put(i, FF, false);
    ASSERT_EQ(memory.get(i), FF);
  }
}
