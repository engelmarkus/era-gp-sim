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

#ifndef ERAGPSIM_PARSER_RISCV_RISCV_PARSER_HPP
#define ERAGPSIM_PARSER_RISCV_RISCV_PARSER_HPP

#include <memory>
#include <string>
#include <vector>

#include "arch/common/architecture.hpp"
#include "arch/common/node-factory-collection.hpp"
#include "core/memory-access.hpp"
#include "parser/common/parser.hpp"

#include "parser/independent/syntax-tree-generator.hpp"

/**
 * Risc-V Parser Class
 *
 * This class parses Risc-V instructions into an abstract syntax tree.
 */
class RiscvParser : public Parser {
 public:
  class RiscvRegex;

  RiscvParser(const Architecture &architecture,
              const MemoryAccess &memoryAccess);

  virtual FinalRepresentation parse(const std::string &text) override;

  virtual const SyntaxInformation getSyntaxInformation() override;

  static const SyntaxTreeGenerator::ArgumentNodeGenerator
      argumentGeneratorFunction;

 protected:
  /**
   * The NodeFactoryCollection used to create the syntax tree nodes
   */
  NodeFactoryCollection _factoryCollection;

  /**
   * Reference to the Architecture this parser compiles for
   */
  Architecture _architecture;

  /**
   * A MemoryAccess for the `validate(_memoryAccess)` call.
   */
  MemoryAccess _memoryAccess;
};

#endif// ERAGPSIM_PARSER_RISCV_RISCV_PARSER_HPP