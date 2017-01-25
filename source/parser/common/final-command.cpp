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

#include "parser/common/final-command.hpp"

// Just constructors and getters.

FinalCommand::FinalCommand(const FinalCommandNodePointer& node,
                           const CodePositionInterval& position,
                           const MemoryAddress& address)
: _node(node), _position(position), _address(address) {
}
const FinalCommandNodePointer& FinalCommand::node() const noexcept {
  return _node;
}
const CodePositionInterval& FinalCommand::position() const noexcept {
  return _position;
}
const MemoryAddress& FinalCommand::address() const noexcept {
  return _address;
}