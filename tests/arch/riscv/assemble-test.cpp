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


#include <gtest/gtest.h>
#include "tests/arch/riscv/test-utils.hpp"

struct AssembleTest32 : RiscvBaseTest {
    using CmdNode = std::unique_ptr<AbstractSyntaxTreeNode>;

    AssembleTest32() {
        load({"rv32i"});
        _factories = getFactories();
    }

    void fillRegisterRegister(CmdNode& cmd, const std::string d0, const std::string r0, const std::string r1) const {
        cmd->addChild(_factories.createRegisterNode(d0));
        cmd->addChild(_factories.createRegisterNode(r0));
        cmd->addChild(_factories.createRegisterNode(r1));
    }

    void fillRegisterImmediate(CmdNode& cmd, const std::string d0, const std::string r0, riscv::signed32_t imm) const {
        cmd->addChild(_factories.createRegisterNode(d0));
        cmd->addChild(_factories.createRegisterNode(r0));
        cmd->addChild(_factories.createImmediateNode(riscv::convert(imm)));
    }

    CmdNode create(const std::string mnemonic) {
        return _factories.createInstructionNode(mnemonic);
    }

private:
    NodeFactoryCollection _factories;
};

TEST_F(AssembleTest32, formatR) {
    //auto cmdR = create("add");
    //fillRegisterRegister(cmdR, "x0", "x0", "x0");
    //cmdR->assemble();
}

TEST_F(AssembleTest32, formatI) {
//    auto cmdI = create("addi");
//    fillRegisterImmediate(cmdI, "x0", "x0", 1);
//    cmdI->assemble();
}
