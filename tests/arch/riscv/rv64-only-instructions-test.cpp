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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "arch/riscv/immediate-node-factory.hpp"
#include "arch/common/register-node.hpp"

#include "dummies.hpp"

using namespace riscv;

namespace {

/**
 * Tests Register-Immediate instructions.
 */
void testRV64OnlyInstructionRI(const std::string& instructionName,
                               uint64_t op1,
                               uint64_t op2,
                               uint64_t expectedResult) {
  // Set up registers
  Register dest, src;
  std::string destId{"dest"}, srcId{"src"};
  MemoryAccess memoryAccess;
  memoryAccess.addRegister(destId, dest);
  memoryAccess.addRegister(srcId, src);

  memoryAccess.setRegisterValue(srcId, riscv::convert<uint64_t>(op1));

  // Set up instruction
  auto instrFactory     = setUpFactory({"rv32i", "rv64i"});
  auto immediateFactory = ImmediateNodeFactory{};
  auto instr            = instrFactory.createInstructionNode(instructionName);

  // Fill instruction with arguments
  ASSERT_TRUE(instr);
  ASSERT_FALSE(instr->validate());
  instr->addChild(std::make_unique<RegisterNode>(destId));
  ASSERT_FALSE(instr->validate());
  instr->addChild(std::make_unique<RegisterNode>(srcId));
  ASSERT_FALSE(instr->validate());
  instr->addChild(
      immediateFactory.createImmediateNode(riscv::convert<uint64_t>(op2)));
  ASSERT_TRUE(instr->validate());

  // Execute
  ASSERT_EQ(instr->getValue(memoryAccess), MemoryValue{});

  // Check result
  MemoryValue result = memoryAccess.getRegisterValue(destId);
  ASSERT_EQ(riscv::convert<uint64_t>(result), expectedResult);
}

/**
 * Tests Register-Register instructions.
 */
void testRV64OnlyInstructionRR(const std::string& instructionName,
                               uint64_t op1,
                               uint64_t op2,
                               uint64_t expectedResult) {
  // Set up registers
  Register dest, src1, src2;
  std::string destId{"dest"}, src1Id{"src1"}, src2Id{"src2"};
  MemoryAccess memoryAccess;
  memoryAccess.addRegister(destId, dest);
  memoryAccess.addRegister(src1Id, src1);
  memoryAccess.addRegister(src2Id, src2);

  memoryAccess.setRegisterValue(src1Id, riscv::convert<uint64_t>(op1));
  memoryAccess.setRegisterValue(src2Id, riscv::convert<uint64_t>(op2));

  // Set up instruction
  auto instrFactory     = setUpFactory({"rv32i", "rv64i"});
  auto immediateFactory = ImmediateNodeFactory{};
  auto instr            = instrFactory.createInstructionNode(instructionName);

  // Fill instruction with arguments
  ASSERT_TRUE(instr);
  ASSERT_FALSE(instr->validate());
  instr->addChild(std::make_unique<RegisterNode>(destId));
  ASSERT_FALSE(instr->validate());
  instr->addChild(std::make_unique<RegisterNode>(src1Id));
  ASSERT_FALSE(instr->validate());
  instr->addChild(std::make_unique<RegisterNode>(src2Id));
  ASSERT_TRUE(instr->validate());

  // Execute
  ASSERT_EQ(instr->getValue(memoryAccess), MemoryValue{});

  // Check result
  MemoryValue result = memoryAccess.getRegisterValue(destId);
  ASSERT_EQ(riscv::convert<uint64_t>(result), expectedResult);
}

/**
 * Tests both Register-Immediate & Register-Register instructions.
 */
void testRV64OnlyInstructions(const std::string& instructionNameRI,
                              const std::string& instructionNameRR,
                              uint64_t op1,
                              uint64_t op2,
                              uint64_t expectedResult) {
  testRV64OnlyInstructionRI(instructionNameRI, op1, op2, expectedResult);
  testRV64OnlyInstructionRR(instructionNameRR, op1, op2, expectedResult);
}

}// Private namespace

TEST(RV64OnlyInstructionsTest, Validation) {
  auto ri = {"addiw", "slliw", "srliw", "sraiw"};
  auto rr = {"addw", "subw", "sllw", "srlw", "sraw"};

  std::string registerId = "";// Not relevant
  auto instrFactory      = setUpFactory({"rv32i", "rv64i"});
  auto immediateFactory  = ImmediateNodeFactory{};

  for (auto& name : ri) {
    // Check if register-immediate command does not allow register-register
    auto instr = instrFactory.createInstructionNode(name);
    instr->addChild(std::make_unique<RegisterNode>(registerId));
    instr->addChild(std::make_unique<RegisterNode>(registerId));
    instr->addChild(std::make_unique<RegisterNode>(registerId));
    ASSERT_FALSE(instr->validate());

    // Boundaries for 12 bit signed integers
    constexpr uint64_t boundary         = 0x7FF;
    constexpr uint64_t overflow         = boundary + 1;
    constexpr uint64_t negativeBoundary = -2048;
    constexpr uint64_t negativeOverflow = negativeBoundary - 1;

    instr = instrFactory.createInstructionNode(name);
    instr->addChild(std::make_unique<RegisterNode>(registerId));
    instr->addChild(std::make_unique<RegisterNode>(registerId));
    instr->addChild(
        immediateFactory.createImmediateNode(riscv::convert<uint64_t>(boundary)));
    ASSERT_TRUE(instr->validate());

    instr = instrFactory.createInstructionNode(name);
    instr->addChild(std::make_unique<RegisterNode>(registerId));
    instr->addChild(std::make_unique<RegisterNode>(registerId));
    instr->addChild(
        immediateFactory.createImmediateNode(riscv::convert<uint64_t>(overflow)));
    ASSERT_FALSE(instr->validate());

    instr = instrFactory.createInstructionNode(name);
    instr->addChild(std::make_unique<RegisterNode>(registerId));
    instr->addChild(std::make_unique<RegisterNode>(registerId));
    instr->addChild(immediateFactory.createImmediateNode(
        riscv::convert<int64_t>(negativeBoundary)));
    ASSERT_TRUE(instr->validate());

    instr = instrFactory.createInstructionNode(name);
    instr->addChild(std::make_unique<RegisterNode>(registerId));
    instr->addChild(std::make_unique<RegisterNode>(registerId));
    instr->addChild(immediateFactory.createImmediateNode(
        riscv::convert<int64_t>(negativeOverflow)));
    ASSERT_FALSE(instr->validate());
  }

  for (auto& name : rr) {
    // Check if register-register command does not allow register-immediate
    auto instr = instrFactory.createInstructionNode(name);
    instr->addChild(std::make_unique<RegisterNode>(registerId));
    instr->addChild(std::make_unique<RegisterNode>(registerId));
    instr->addChild(
        immediateFactory.createImmediateNode(riscv::convert<uint64_t>(0)));
    ASSERT_FALSE(instr->validate());
  }
}

TEST(RV64OnlyInstructionsTest, Add) {
  std::string ri = "addiw";
  std::string rr = "addw";

  // What was 1+1 again?
  testRV64OnlyInstructions(ri, rr, 1, 1, 2);
  testRV64OnlyInstructions(ri, rr, 42, 1, 43);
  testRV64OnlyInstructions(ri, rr, 1, 42, 43);
  // Check if upper 32 bits are ignored
  testRV64OnlyInstructions(ri, rr, (uint64_t{1} << 32) + 1, 1, 2);
  // Check if overflow is ignored
  testRV64OnlyInstructions(ri, rr, UINT32_MAX, 1, 0);
  // Check if sign extension works
  testRV64OnlyInstructions(ri, rr, UINT32_MAX, 0, UINT64_MAX);
}

TEST(RV64OnlyInstructionsTest, Sub) {
  std::string rr = "subw";

  // What was 1-1 again?
  testRV64OnlyInstructionRR(rr, 1, 1, 0);
  // Check if uppper 32 bits are ignored
  testRV64OnlyInstructionRR(rr, (uint64_t{1} << 32) + 1, 1, 0);
  // Check if overflow is ignored
  testRV64OnlyInstructionRR(rr, uint64_t{1} << 31, 1, INT32_MAX);
  // Check if sign extension works
  testRV64OnlyInstructionRR(rr, -1, 0, UINT64_MAX);
}

TEST(RV64OnlyInstructionsTest, Sll) {
  std::string ri = "slliw";
  std::string rr = "sllw";

  // Nothing special
  testRV64OnlyInstructions(ri, rr, 1, 24, uint64_t{1} << 24);
  // Check if only lower 5 bits of shift amount are used
  // That means, that nothing will happen, if shift amount is
  // bigger than 31
  testRV64OnlyInstructions(ri, rr, 1, 32, 1);
  // Check if sign extension works
  testRV64OnlyInstructions(ri, rr, -1, 0, UINT64_MAX);
  testRV64OnlyInstructions(ri, rr, 1, 31,0xFFFFFFFF80000000);
}

TEST(RV64OnlyInstructionsTest, Srl) {
  std::string ri = "srliw";
  std::string rr = "srlw";

  // Nothing special
  testRV64OnlyInstructions(ri, rr, uint64_t{1} << 31, 31, 1);
  // Check if only lower 5 bits of shift amount are used
  // That means, that nothing will happen, if shift amount is
  // bigger than 31
  testRV64OnlyInstructions(ri, rr, 1, 32, 1);
  // Check if sign extension works
  testRV64OnlyInstructions(ri, rr, -1, 0, UINT64_MAX);
}

TEST(RV64OnlyInstructionsTest, Sra) {
  std::string ri = "sraiw";
  std::string rr = "sraw";

  // Nothing special
  testRV64OnlyInstructions(ri, rr, 4, 2, 1);
  // Check if only lower 5 bits of shift amount are used
  // That means, that nothing will happen, if shift amount is
  // bigger than 31
  testRV64OnlyInstructions(ri, rr, 4, 32, 4);
  // Check if arithmetic shift/sign extension works
  testRV64OnlyInstructions(ri, rr, -4, 2, UINT64_MAX);// max because thats -1
}