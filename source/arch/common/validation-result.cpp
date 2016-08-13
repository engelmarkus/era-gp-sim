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

#include "arch/common/validation-result.hpp"

ValidationResult::ValidationResult(Type type) : _type(type) {
}

bool ValidationResult::isSuccess() const {
  return _type == Type::SUCCESS;
}

ValidationResult::Type ValidationResult::getType() const {
  return _type;
}

ValidationResult& ValidationResult::addInt(int parameter) noexcept {
  _ints.push_back(parameter);
  return *this;
}

ValidationResult& ValidationResult::addBool(bool parameter) noexcept {
  _bools.push_back(parameter);
  return *this;
}

int ValidationResult::getInt(int index) const noexcept {
  if (index < 0 || index >= _ints.size()) {
    return 0;
  }

  return _ints[index];
}

bool ValidationResult::getBool(int index) const noexcept {
  if (index < 0 || index >= _bools.size()) {
    return false;
  }

  return _bools[index];
}
