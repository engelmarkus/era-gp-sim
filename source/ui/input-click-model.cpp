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

#include "ui/input-click-model.hpp"

#include "core/conversions.hpp"

InputClickModel::InputClickModel(QQmlContext *context,
                                 MemoryAccess memoryAccess)
: QObject(), _context(context), _memoryAccess(memoryAccess), _start(0) {
  _context->setContextProperty("inputClickMod", this);
}

void InputClickModel::newClick(unsigned int x, unsigned int y) {
  // save in Memory as 2 bytes
  auto xMemory = conversions::convert(x, 8);
  auto yMemory = conversions::convert(y, 8);

  _memoryAccess.putMemoryValueAt(_start, xMemory);
  _memoryAccess.putMemoryValueAt(_start + 1, yMemory);
}

void InputClickModel::setStart(unsigned int start) {
  if (_memoryAccess.getMemorySize().get() > start + 2) {
    _start = start;
  }
}

QString InputClickModel::getStart() {
  return QString::fromStdString(std::to_string(_start));
}
