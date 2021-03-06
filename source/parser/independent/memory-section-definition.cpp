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

#include "parser/independent/memory-section-definition.hpp"

#include <cstddef>
#include <string>

#include "common/assert.hpp"

MemorySectionDefinition::MemorySectionDefinition(const std::string& name,
                                                 size_t sectionAlignment,
                                                 size_t dataAlignment)
: _name(name)
, _sectionAlignment(sectionAlignment)
, _dataAlignment(dataAlignment) {
  assert::that(_sectionAlignment > 0);
  assert::that(_dataAlignment > 0);

  // Important: the section alignment (if this even makes sense) must be a
  // multiple of the data alignment.
  assert::that(_sectionAlignment % _dataAlignment == 0);
}

MemorySectionDefinition::MemorySectionDefinition(const std::string& name,
                                                 size_t alignment)
: MemorySectionDefinition(name, alignment, alignment) {
}

const std::string& MemorySectionDefinition::name() const noexcept {
  return _name;
}

size_t MemorySectionDefinition::sectionAlignment() const noexcept {
  return _sectionAlignment;
}

size_t MemorySectionDefinition::dataAlignment() const noexcept {
  return _dataAlignment;
}
