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

#ifndef ERAGPSIM_PARSER_INTERMEDIATE_MACRO_INSTRUCTION_HPP
#define ERAGPSIM_PARSER_INTERMEDIATE_MACRO_INSTRUCTION_HPP

#include "parser/intermediate-operation.hpp"

class CompileErrorAnnotator;
class IntermediateInstruction;
class MacroDirective;
class MemoryAllocator;

/**
 * Wrapper instruction for an instanced macro.
 */
class IntermediateMacroInstruction : public IntermediateOperation {
 public:
  IntermediateMacroInstruction(const IntermediateInstruction&,
                               const MacroDirective&,
                               MacroDirectiveTable& macroTable,
                               const CompileErrorAnnotator& annotator);

  virtual void execute(const ExecuteImmutableArguments& immutable,
                       const CompileErrorAnnotator& annotator,
                       FinalRepresentation& finalRepresentator,
                       MemoryAccess& memoryAccess);

  virtual void
  enhanceSymbolTable(const EnhanceSymbolTableImmutableArguments& immutable,
                     const CompileErrorAnnotator& annotator,
                     SymbolGraph& graph);

  virtual void allocateMemory(const PreprocessingImmutableArguments& immutable,
                              const CompileErrorAnnotator& annotator,
                              MemoryAllocator& allocator,
                              SectionTracker& tracker);

  /**
   * Replaces `IntermediateInstruction`s with `IntermediateMacroInstructions` in
   * iterable containers of `IntermediateOperationPointer`s.
   */
  static void replaceWithMacros(CommandIterator begin,
                                CommandIterator end,
                                MacroDirectiveTable& macroTable,
                                const CompileErrorAnnotator& annotator);

  virtual std::string toString() const;

  virtual Type getType() const;

 private:
  CommandList _operations;
  int _firstInstruction = -1;
};


#endif
