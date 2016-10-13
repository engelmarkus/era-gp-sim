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

#include <gtest/gtest.h>
#include <cstdint>
#include <iostream>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

#include "arch/common/abstract-syntax-tree-node.hpp"
#include "arch/common/immediate-node.hpp"
#include "arch/common/immediate-node.hpp"
#include "arch/common/instruction-format.hpp"
#include "arch/common/instruction-key.hpp"
#include "arch/common/instruction-set.hpp"
#include "arch/common/register-node.hpp"
#include "arch/riscv/formats.hpp"
#include "arch/riscv/instruction-node.hpp"
#include "arch/riscv/integer-instructions.hpp"
#include "core/memory-value.hpp"

using namespace riscv;

// testing the immediate transformations
struct ImmediateFormatTestFixture : public ::testing::Test {
  ImmediateFormatTestFixture() {
    for (int i = 0; i < 6; i++) {
      auto vect = std::vector<uint8_t>(std::begin(v[i]), std::end(v[i]));
      valueP[i] = new MemoryValue(vect, 8);
    }
  }

  ~ImmediateFormatTestFixture() {
    for (int i = 0; i < 6; i++) delete valueP[i];
    delete[] valueP;
  }

  uint8_t v[6][4] = {{0x78, 0xEF, 0xCD, 0xAB}, {0x00, 0x00, 0x07, 0x8E},
                     {0x00, 0x00, 0x07, 0x9B}, {0x00, 0x00, 0x0F, 0x9A},
                     {0X78, 0xEF, 0xC0, 0x00}, {0x00, 0x0F, 0xC7, 0x8E}};
  MemoryValue **valueP = new MemoryValue *[6];
};

TEST_F(ImmediateFormatTestFixture, IFormat) {
  immToI(*valueP[0]);
  ASSERT_EQ(*valueP[1], *valueP[0]);
}

TEST_F(ImmediateFormatTestFixture, SFormat) {
  immToS(*valueP[0]);
  ASSERT_EQ(*valueP[2], *valueP[0]);
}

TEST_F(ImmediateFormatTestFixture, BFormat) {
  immToB(*valueP[0]);
  ASSERT_EQ(*valueP[3], *valueP[0]);
}

TEST_F(ImmediateFormatTestFixture, UFormat) {
  immToU(*valueP[0]);
  ASSERT_EQ(*valueP[4], *valueP[0]);
}

TEST_F(ImmediateFormatTestFixture, JFormat) {
  immToJ(*valueP[0]);
  ASSERT_EQ(*valueP[5], *valueP[0]);
}

struct InstructionFormatTestFixture : public ::testing::Test {
  InstructionFormatTestFixture() {
    instructionSet.addInstructions(InstructionSet(
        {{"add", InstructionKey({{"opcode", 6}, {"function", 3}}), "R"},
         {"sub", InstructionKey({{"opcode", 9}, {"function", 3}}), "I"},
         {"beq", InstructionKey({{"opcode", 7}, {"function", 1}}), "SB"},
         {"uinst", InstructionKey({{"opcode", 3}, {"function", 5}}), "U"}}));
  }

  ~InstructionFormatTestFixture() {}

  InstructionSet instructionSet;
};

// testing the different formats
TEST_F(InstructionFormatTestFixture, RFormat) {
  using Operands = AddInstructionNode<uint32_t>::Operands;
  auto addInfo = instructionSet.getInstruction("add");
  AddInstructionNode<uint32_t> addInstr(addInfo, Operands::REGISTERS);

  AbstractSyntaxTreeNode::Node r1(new RegisterNode("1"));
  AbstractSyntaxTreeNode::Node r2(new RegisterNode("2"));
  AbstractSyntaxTreeNode::Node rd(new RegisterNode("3"));
  addInstr.addChild(std::move(rd));
  addInstr.addChild(std::move(r2));
  addInstr.addChild(std::move(r1));

  std::cout << addInstr.assemble() << std::endl;
}

TEST_F(InstructionFormatTestFixture, IFormat) {
  using Operands = AddInstructionNode<uint32_t>::Operands;
  auto addInfo = instructionSet.getInstruction("sub");
  AddInstructionNode<uint32_t> addInstr(addInfo, Operands::IMMEDIATES);

  MemoryValue val(4 * 8);
  val.put(22, true);
  val.put(23, true);
  AbstractSyntaxTreeNode::Node imm(new ImmediateNode(val));
  AbstractSyntaxTreeNode::Node r2(new RegisterNode("2"));
  AbstractSyntaxTreeNode::Node rd(new RegisterNode("3"));
  addInstr.addChild(std::move(rd));
  addInstr.addChild(std::move(r2));
  addInstr.addChild(std::move(imm));

  std::cout << addInstr.assemble() << std::endl;
}

TEST_F(InstructionFormatTestFixture, SBFormat) {
  using Operands = AddInstructionNode<uint32_t>::Operands;
  auto beqInfo = instructionSet.getInstruction("beq");
  AddInstructionNode<uint32_t> beqInstr(beqInfo, Operands::IMMEDIATES);

  MemoryValue val(4 * 8);
  val.put(22, true);
  val.put(23, true);
  AbstractSyntaxTreeNode::Node imm(new ImmediateNode(val));
  AbstractSyntaxTreeNode::Node r2(new RegisterNode("2"));
  AbstractSyntaxTreeNode::Node r1(new RegisterNode("3"));
  beqInstr.addChild(std::move(r1));
  beqInstr.addChild(std::move(r2));
  beqInstr.addChild(std::move(imm));

  std::cout << beqInstr.assemble() << std::endl;
}

TEST_F(InstructionFormatTestFixture, UFormat) {
  using Operands = AddInstructionNode<uint32_t>::Operands;
  auto uInfo = instructionSet.getInstruction("uinst");
  AddInstructionNode<uint32_t> uInst(uInfo, Operands::IMMEDIATES);

  MemoryValue val(4 * 8);
  val.put(22, true);
  val.put(23, true);
  AbstractSyntaxTreeNode::Node imm(new ImmediateNode(val));
  AbstractSyntaxTreeNode::Node rd(new RegisterNode("3"));
  uInst.addChild(std::move(rd));
  uInst.addChild(std::move(imm));

  std::cout << uInst.assemble() << std::endl;
}