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

#include "parser/independent/intermediate-representator.hpp"

#include "arch/common/architecture.hpp"
#include "core/memory-access.hpp"
#include "parser/common/compile-error-list.hpp"
#include "parser/common/final-command.hpp"
#include "parser/common/final-representation.hpp"
#include "parser/common/macro-information.hpp"
#include "parser/independent/enhance-symbol-table-immutable-arguments.hpp"
#include "parser/independent/execute-immutable-arguments.hpp"
#include "parser/independent/intermediate-macro-instruction.hpp"
#include "parser/independent/macro-directive-table.hpp"
#include "parser/independent/memory-allocator.hpp"
#include "parser/independent/preprocessing-immutable-arguments.hpp"
#include "parser/independent/section-tracker.hpp"
#include "parser/independent/symbol-graph-evaluation.hpp"
#include "parser/independent/symbol-graph.hpp"
#include "parser/independent/symbol-replacer.hpp"
#include "parser/independent/transformation-parameters.hpp"

MacroInformationVector IntermediateRepresentator::generateMacroInformation() {
  MacroInformationVector output;
  for (const auto& command : _commandList) {
    if (command->getType() == IntermediateOperation::Type::MACRO_INSTRUCTION) {
      auto macroCode =
          static_cast<IntermediateMacroInstruction&>(*command).toString();

      auto info = MacroInformation(macroCode, command->positionInterval());

      output.emplace_back(info);
    }
  }
  return output;
}

IntermediateRepresentator::IntermediateRepresentator()
: _commandList(), _currentOutput(nullptr) {
}

void IntermediateRepresentator::insertCommandPointer(
    const IntermediateOperationPointer& command, CompileErrorList& errors) {
  // We got to handle the three target selector cases right here.
  if (command->newTarget() == TargetSelector::THIS) {
    // If we want the current command as new target, we set it like so.
    if (_currentOutput) {
      // Nested macros are not supported.
      errors.pushError(command->name().positionInterval(),
                       "Error, nested macros are not supported.");
    }
    _currentOutput = command;
  } else {
    if (command->newTarget() == TargetSelector::MAIN) {
      // For the main selector, we may also insert the old command (otherwise
      // it and its sub commands might be lost).
      if (!_currentOutput) {
        // Classic bracket-forgot-to-close problem.
        errors.pushError(command->name().positionInterval(),
                         "The start directive of the macro is missing.");
      }

      auto oldOutput = _currentOutput;
      _currentOutput = nullptr;

      internalInsertCommand(oldOutput);
    }

    // Finally, we may insert our handed-over command.
    internalInsertCommand(command);
  }
}

namespace {
bool evaluateGraph(const SymbolGraphEvaluation& graphEvaluation,
                   CompileErrorList& errors) {
  if (!graphEvaluation.valid()) {
    if (!graphEvaluation.invalidNames().empty()) {
      for (auto index : graphEvaluation.invalidNames()) {
        const auto& symbol = graphEvaluation.symbols()[index];
        errors.pushError(
            symbol.name().positionInterval(),
            "The name '%1' is not a valid name for a constant, label etc.",
            symbol.name().string());
      }
    }
    if (!graphEvaluation.duplicates().empty()) {
      for (const auto& indexGroup : graphEvaluation.duplicates()) {
        for (auto index : indexGroup) {
          const auto& symbol = graphEvaluation.symbols()[index];
          errors.pushError(symbol.name().positionInterval(),
                           "The name '%1' exists more than once in the program",
                           symbol.name().string());
        }
      }
    }
    if (!graphEvaluation.sampleCycle().empty()) {
      std::string displayString;
      for (auto index : graphEvaluation.sampleCycle()) {
        const auto& symbol = graphEvaluation.symbols()[index];
        displayString += "'" + symbol.name().string() + "' -> ";
      }
      displayString += "...";
      for (auto index : graphEvaluation.sampleCycle()) {
        const auto& symbol = graphEvaluation.symbols()[index];
        errors.pushError(
            symbol.name().positionInterval(),
            "Symbol '%1' is part of an infinite reference cycle, going like %2",
            symbol.name().string(),
            displayString);
      }
    }
    return false;
  } else {
    return true;
  }
}

bool checkMemorySize(
    std::size_t allocatedSize,
    std::size_t allowedSize,
    const IntermediateOperationPointer& firstMemoryExceedingOperation,
    CompileErrorList& errors) {
  if (allocatedSize > allowedSize) {
    if (firstMemoryExceedingOperation) {
      errors.pushError(
          CodePositionInterval(
              firstMemoryExceedingOperation->positionInterval()),
          "From this operation on, including it, there is too much memory "
          "allocated in total: %1 requested, maximum is %2"
          " (please note: because of aligning memory, the first value "
          "might be actually bigger than the memory allocated)",
          std::to_string(allocatedSize),
          std::to_string(allowedSize));
    } else {
      errors.pushError(
          CodePositionInterval(CodePosition(0), CodePosition(0, 2)),
          "Too much memory allocated: %1 requested, maximum is %2"
          " (please note: because of aligning memory, the first value "
          "might be actually bigger than the memory allocated)",
          std::to_string(allocatedSize),
          std::to_string(allowedSize));
    }
    return false;
  }
  return true;
}
};

FinalRepresentation
IntermediateRepresentator::transform(const TransformationParameters& parameters,
                                     const CompileErrorList& parsingErrors,
                                     MemoryAccess& memoryAccess) {
  auto errors = parsingErrors;
  if (_currentOutput) {
    errors.pushError(_currentOutput->positionInterval(),
                     "Macro not closed. Missing a macro end directive?");
  }

  auto preprocessingArguments = PreprocessingImmutableArguments(
      parameters.architecture(), parameters.generator());

  auto macroTable = MacroDirectiveTable();
  for (const auto& command : _commandList) {
    command->precompile(preprocessingArguments, errors, macroTable);
  }

  IntermediateMacroInstruction::replaceWithMacros(
      _commandList.begin(), _commandList.end(), macroTable, errors);

  auto macroList = generateMacroInformation();

  auto allocator = MemoryAllocator(parameters.allocator());
  auto tracker = SectionTracker();

  auto allowedSize = memoryAccess.getMemorySize().get();
  auto firstMemoryExceedingOperation = IntermediateOperationPointer(nullptr);

  for (const auto& command : _commandList) {
    command->allocateMemory(preprocessingArguments, errors, allocator, tracker);
    if (allocator.estimateSize() > allowedSize &&
        !firstMemoryExceedingOperation) {
      firstMemoryExceedingOperation = command;
    }
  }

  auto allocatedSize = allocator.calculatePositions();

  auto graph = SymbolGraph();
  auto symbolTableArguments =
      EnhanceSymbolTableImmutableArguments(preprocessingArguments, allocator);
  for (const auto& command : _commandList) {
    command->enhanceSymbolTable(symbolTableArguments, errors, graph);
  }

  auto graphEvaluation = graph.evaluate();
  auto graphValid = evaluateGraph(graphEvaluation, errors);
  auto memoryValid = checkMemorySize(
      allocatedSize, allowedSize, firstMemoryExceedingOperation, errors);
  if (!(graphValid || memoryValid)) {
    return FinalRepresentation({}, errors, macroList);
  }

  auto replacer = SymbolReplacer(graphEvaluation);
  auto executeArguments =
      ExecuteImmutableArguments(symbolTableArguments, replacer);
  auto commandOutput = FinalCommandVector();
  for (const auto& command : _commandList) {
    command->execute(executeArguments, errors, commandOutput, memoryAccess);
  }

  return FinalRepresentation(commandOutput, errors, macroList);
}

void IntermediateRepresentator::internalInsertCommand(
    const IntermediateOperationPointer& pointer) {
  // Of course, it should be valid and is allowed to be inserted.
  if (pointer && pointer->shouldInsert()) {
    // We got to decide if there is an alternative output.
    if (_currentOutput) {
      _currentOutput->insert(pointer);
    } else {
      _commandList.emplace_back(pointer);
    }
  }
}
