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
* along with this program. If not, see <http://www.gnu.org/licenses/>.*/

#include "arch/common/binarydata-node.hpp"
#include "arch/riscv/properties.hpp"
#include "arch/riscv/register-node.hpp"
#include "arch/riscv/utility.hpp"

#include "tests/arch/riscv/base-fixture.hpp"

using namespace riscv;

struct SimulatorInstructionTest : public riscv::BaseFixture {};

TEST_F(SimulatorInstructionTest, SIMUCRASH) {
  loadArchitecture({"rv32i"});
  auto& memoryAccess = getMemoryAccess();

  std::string customErrorMsg = "Oh no, something went wrong :(";

  auto instr = factories.createInstructionNode("simucrash");
  ASSERT_FALSE(instr->validate(memoryAccess).isSuccess());
  instr->addChild(factories.createDataNode(customErrorMsg));
  ASSERT_TRUE(instr->validate(memoryAccess).isSuccess());

  // test runtime behaviour
  ValidationResult result = instr->validateRuntime(memoryAccess);
  ASSERT_FALSE(result.isSuccess());
  std::string msg = result.getMessage().getBaseString();
  for (auto& opPtr : result.getMessage().getOperands()) {
    msg += opPtr->getBaseString();// no real replacement neccessary
  }
  // check that the custom error message is somewhere in the runtime message
  ASSERT_TRUE(msg.find(customErrorMsg) != std::string::npos);
}

TEST_F(SimulatorInstructionTest, SIMUCRASH_validation) {
  using Type = AbstractSyntaxTreeNode::Type;
  loadArchitecture({"rv32i", "rv64i"});
  auto& memoryAccess = getMemoryAccess();


  auto twoOpInstruction = factories.createInstructionNode("simucrash");
  twoOpInstruction->addChild(factories.createDataNode(""));
  ASSERT_TRUE(twoOpInstruction->validate(memoryAccess).isSuccess());
  twoOpInstruction->addChild(factories.createDataNode(""));
  ASSERT_FALSE(twoOpInstruction->validate(memoryAccess).isSuccess());

  auto instrReg = factories.createInstructionNode("simucrash");
  instrReg->addChild(factories.createRegisterNode("x0"));
  ASSERT_FALSE(instrReg->validate(memoryAccess).isSuccess());
  auto instrImm = factories.createInstructionNode("simucrash");
  instrImm->addChild(factories.createImmediateNode(MemoryValue(64)));
  ASSERT_FALSE(instrImm->validate(memoryAccess).isSuccess());
}

TEST_F(SimulatorInstructionTest, SIMUSLEEP) {
  constexpr riscv::signed32_t sleeptime = 200;
  loadArchitecture({"rv32i"});
  auto& memoryAccess = getMemoryAccess();

  auto instr = factories.createInstructionNode("simusleep");
  instr->addChild(factories.createImmediateNode(
      riscv::convert<riscv::signed32_t>(sleeptime)));

  ASSERT_TRUE(instr->validate(memoryAccess).isSuccess());

  auto start = std::chrono::high_resolution_clock::now();
  instr->getValue(memoryAccess);
  auto end = std::chrono::high_resolution_clock::now();
  EXPECT_LE(std::chrono::milliseconds(sleeptime),
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
}

TEST_F(SimulatorInstructionTest, SIMUSLEEP_validation) {
  loadArchitecture({"rv32i", "rv64i"});
  auto& memoryAccess = getMemoryAccess();

  auto twoOpInstr = factories.createInstructionNode("simusleep");
  ASSERT_FALSE(twoOpInstr->validate(memoryAccess).isSuccess());
  twoOpInstr->addChild(
      factories.createImmediateNode(riscv::convert<riscv::signed32_t>(42)));
  ASSERT_TRUE(twoOpInstr->validate(memoryAccess).isSuccess());
  twoOpInstr->addChild(
      factories.createImmediateNode(riscv::convert<riscv::signed32_t>(42)));
  ASSERT_FALSE(twoOpInstr->validate(memoryAccess).isSuccess());
}
