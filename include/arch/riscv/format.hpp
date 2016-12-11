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

#ifndef ERAGPSIM_ARCH_RISCV_FORMATS_HPP
#define ERAGPSIM_ARCH_RISCV_FORMATS_HPP

#include <functional>
#include <string>
#include <vector>

#include "core/memory-value.hpp"

class InstructionInformation;
class InstructionKey;

namespace riscv {
namespace Format {

using Arguments = std::vector<MemoryValue>;
using AssemblerFunction =
    std::function<MemoryValue(InstructionKey, const Arguments&)>;

/**
 * Returns the appropriate assembly function for the given format string.
 * \param  format The name of the format (e.g. "R").
 * \return The assembler function for the given format.
 */
AssemblerFunction getAssembler(const std::string& format);

/**
 * Assembles the arguments for the given instruction information.
 *
 * The format is extracted from the information object.
 *
 * \param  instructionInformation The instruction information
 *                                to assemble for.
 * \param  arguments The arguments to assemble.
 * @return The assembled memory value.
 */
MemoryValue assemble(const InstructionInformation& instructionInformation,
                     const Arguments& arguments);

/*
 * funct7 | rs2 | rs1 | funct3 | rd | opcode
 */
MemoryValue R(const InstructionKey& key, const Arguments& arguments);

/*
 * imm[11:0] | rs1 | funct3 | rd | opcode
 */
MemoryValue I(const InstructionKey& key, const Arguments& arguments);

/*
 * imm[11:5] | rs2 | rs1 | funct3 | imm[4:0] | opcode
 */
MemoryValue S(const InstructionKey& key, const Arguments& arguments);

/*
 * imm[31:12] | rd | opcode
 */
MemoryValue U(const InstructionKey& key, const Arguments& arguments);

/*
 * The same as SFormat except the immediate encoding.
 */
MemoryValue SB(const InstructionKey& key, const Arguments& arguments);

/*
 * The same as UFormat except the immediate encoding.
 */
MemoryValue UJ(const InstructionKey& key, const Arguments& arguments);
}
}

#endif /* ERAGPSIM_ARCH_RISCV_FORMATS_HPP */
