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

#include "core/parsing-and-execution-unit.hpp"

#include "arch/common/architecture.hpp"
#include "arch/common/unit-information.hpp"
#include "common/assert.hpp"
#include "core/conversions.hpp"
#include "parser/parser-factory.hpp"
#include "parser/parser-mode.hpp"

ParsingAndExecutionUnit::ParsingAndExecutionUnit(
    std::weak_ptr<Scheduler> &&scheduler,
    MemoryAccess memoryAccess,
    Architecture architecture,
    SharedCondition stopCondition,
    std::string parserName)
: Servant(std::move(scheduler))
, _parser(ParserFactory::createParser(architecture, memoryAccess, parserName))
, _stopCondition(stopCondition)
, _finalRepresentation()
, _addressCommandMap()
, _lineCommandCache()
, _memoryAccess(memoryAccess)
, _breakpoints()
, _syntaxInformation(_parser->getSyntaxInformation())
, _setContextInformation([](const std::vector<ContextInformation> &x) {})
, _setErrorList([](const std::vector<CompileError> &x) {})
, _throwError(([](const std::string &x, const std::vector<std::string> &y) {}))
, _setMacroList(([](const std::vector<MacroInformation> &x) {}))
, _setCurrentLine([](size_t x) {})
, _executionStopped([]() {}) {
  // find the RegisterInformation object of the program counter
  for (UnitInformation unitInfo : architecture.getUnits()) {
    if (unitInfo.hasSpecialRegister(
            RegisterInformation::Type::PROGRAM_COUNTER)) {
      _programCounter = unitInfo.getSpecialRegister(
          RegisterInformation::Type::PROGRAM_COUNTER);
    }
  }
}

void ParsingAndExecutionUnit::execute() {
  if (_finalRepresentation.hasErrors()) {
    _executionStopped();
    return;
  }
  _stopCondition->reset();
  size_t nextNode = _findNextNode();
  while (true) {
    if (_stopCondition->getFlag()) break;
    if (nextNode >= _finalRepresentation.commandList.size()) break;
    if (!_executeNode(nextNode)) break;
    nextNode = _updateLineNumber(nextNode);
  }
  _executionStopped();
}

void ParsingAndExecutionUnit::executeNextLine() {
  _stopCondition->reset();
  size_t nextNode = _findNextNode();
  if (_executeNode(nextNode)) {
    _updateLineNumber(nextNode);
  }
  _executionStopped();
}

void ParsingAndExecutionUnit::executeToBreakpoint() {
  // check if there are parser errors
  if (_finalRepresentation.hasErrors()) {
    _executionStopped();
    return;
  }
  // reset stop flag
  _stopCondition->reset();
  // find the index of the next node and loop through the instructions
  size_t nextNode = _findNextNode();
  while (true) {
    if (_stopCondition->getFlag()) break;
    if (nextNode >= _finalRepresentation.commandList.size()) break;
    if (!_executeNode(nextNode)) break;
    nextNode = _updateLineNumber(nextNode);
    // check if there is a brekpoint on the next line
    if (nextNode < _finalRepresentation.commandList.size()) {
      auto &nextCommand = _finalRepresentation.commandList[nextNode];
      size_t nextLine = nextCommand.position.lineStart();
      if (_breakpoints.count(nextLine) > 0) {
        // we reached a breakpoint
        break;
      }
    }
  }
  _executionStopped();
}

void ParsingAndExecutionUnit::setExecutionPoint(size_t line) {
  MemoryValue address;
  bool foundMatchingLine = false;
  int displayLine = line;
  // try to find the line in the cache
  auto iterator = _lineCommandCache.find(line);
  if (iterator != _lineCommandCache.end()) {
    address = iterator->second;
    foundMatchingLine = true;
  } else {
    for (const auto &command : _finalRepresentation.commandList) {
      if (command.position.lineStart() >= line) {
        // this command is on the given line, save the address in the cache.
        displayLine = command.position.lineStart();
        auto size = _programCounter.getSize();
        address = conversions::convert(command.address, size);
        _lineCommandCache.emplace(displayLine, address);
        foundMatchingLine = true;
        break;
      }
    }
  }
  if (!foundMatchingLine) {
    // no command found on this line
    return;
  }
  // a command on this line was found, set the program counter and the line in
  // the ui.
  _memoryAccess.putRegisterValue(_programCounter.getName(), address);
  _setCurrentLine(displayLine);
}

void ParsingAndExecutionUnit::parse(std::string code) {
  // delete old assembled program in memory
  if (!_finalRepresentation.hasErrors()) {
    for (const auto &command : _finalRepresentation.commandList) {
      // create a empty MemoryValue as long as the command
      MemoryValue zero(command.node->assemble().getSize());
      _memoryAccess.putMemoryValueAt(command.address, zero);
      _memoryAccess.removeMemoryProtection(command.address, zero.getSize() / 8);
    }
  }
  // parse the new code and save the final representation
  _finalRepresentation = _parser->parse(code, ParserMode::COMPILE);
  _addressCommandMap = _finalRepresentation.createMapping();
  _lineCommandCache.clear();
  // update the error list of the ui
  _setErrorList(_finalRepresentation.errorList);
  _setMacroList(_finalRepresentation.macroList);
  // assemble commands into memory
  if (!_finalRepresentation.hasErrors()) {
    for (const auto &command : _finalRepresentation.commandList) {
      auto assemble = command.node->assemble();
      _memoryAccess.putMemoryValueAt(command.address, assemble);
      _memoryAccess.makeMemoryProtected(command.address,
                                        assemble.getSize() / 8);
    }
  }
}

bool ParsingAndExecutionUnit::setBreakpoint(size_t line) {
  return _breakpoints.insert(line).second;
}

void ParsingAndExecutionUnit::deleteBreakpoint(size_t line) {
  _breakpoints.erase(line);
}

SyntaxInformation::TokenIterable
ParsingAndExecutionUnit::getSyntaxRegex(SyntaxInformation::Token token) const {
  return _syntaxInformation.getSyntaxRegex(token);
}

void ParsingAndExecutionUnit::setSetContextInformationCallback(
    ListCallback<ContextInformation> callback) {
  _setContextInformation = callback;
}

void ParsingAndExecutionUnit::setSetErrorListCallback(
    ListCallback<CompileError> callback) {
  _setErrorList = callback;
}

void ParsingAndExecutionUnit::setThrowErrorCallback(
    Callback<const std::string &, const std::vector<std::string> &> callback) {
  _throwError = callback;
}

void ParsingAndExecutionUnit::setSetMacroListCallback(
    ListCallback<MacroInformation> callback) {
  _setMacroList = callback;
}

void ParsingAndExecutionUnit::setSetCurrentLineCallback(
    Callback<size_t> callback) {
  _setCurrentLine = callback;
}

void ParsingAndExecutionUnit::setExecutionStoppedCallback(Callback<> callback) {
  _executionStopped = callback;
}

size_t ParsingAndExecutionUnit::_findNextNode() {
  // get the value of the program counter and convert it to a std::size_t
  std::string programCounterName = _programCounter.getName();
  MemoryValue programCounterValue =
      _memoryAccess.getRegisterValue(programCounterName).get();
  size_t nextInstructionAddress =
      conversions::convert<size_t>(programCounterValue);
  if (nextInstructionAddress == 0) {
    // if the program counter is 0, execute the first instruction
    return 0;
  }
  // find the instruction address from the program counter value
  auto iterator = _addressCommandMap.find(nextInstructionAddress);
  if (iterator == _addressCommandMap.end()) {
    // if there is no instruction (end of program reached), return a value that
    // stops the execution loop
    return _finalRepresentation.commandList.size();
  }
  return iterator->second;
}

bool ParsingAndExecutionUnit::_executeNode(size_t nodeIndex) {
  if (_finalRepresentation.hasErrors()) return false;
  if (nodeIndex >= _finalRepresentation.commandList.size()) return false;

  // reference to avoid copying a unique_ptr
  auto &currentCommand = _finalRepresentation.commandList[nodeIndex];
  // check for runtime errors
  auto validationResult = currentCommand.node->validateRuntime(_memoryAccess);
  if (!validationResult.isSuccess()) {
    // notify the ui of a runtime error
    _throwError(validationResult.getMessage().getBaseString(), {});
    return false;
  }
  // update the current line in the ui (pre-execution)
  _setCurrentLine(currentCommand.position.lineStart());

  MemoryValue programCounterValue =
      currentCommand.node->getValue(_memoryAccess);
  _memoryAccess.putRegisterValue(_programCounter.getName(),
                                 programCounterValue);
  return true;
}

size_t ParsingAndExecutionUnit::_updateLineNumber(size_t currentNode) {
  assert::that(currentNode < _finalRepresentation.commandList.size());

  size_t nextNode = _findNextNode();
  // if there is no command after this, advance the line position by one.
  auto &currentCommand = _finalRepresentation.commandList[currentNode];
  size_t nextLine = currentCommand.position.lineStart() + 1;
  if (nextNode < _finalRepresentation.commandList.size()) {
    // if there is another command, set the next line to its position
    auto &nextCommand = _finalRepresentation.commandList[nextNode];
    nextLine = nextCommand.position.lineStart();
  }
  _setCurrentLine(nextLine);
  return nextNode;
}
