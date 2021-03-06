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

#include <QKeySequence>
#include "ui/input-key-model.hpp"
#include "core/conversions.hpp"

InputKeyModel::InputKeyModel(QQmlContext* context,
                                   MemoryAccess memoryAccess)
: QObject(), _context(context), _start(0), _memoryAccess(memoryAccess) {
  _context->setContextProperty("inputKeyMod", this);
}

void InputKeyModel::keyPressed(unsigned int id) {
  // save id at start in Memory
  auto memoryValue = conversions::convert(id, 32);
  _memoryAccess.putMemoryValueAt(_start, memoryValue);
}

QString InputKeyModel::getKeyDescription(Qt::Key key) const {
    return QKeySequence(key).toString();
}

QString InputKeyModel::getStart() {
  return QString::fromStdString(std::to_string(_start));
}

void InputKeyModel::setStart(unsigned int start) {
  if (_memoryAccess.getMemorySize().get() > start + 2) {
    _start = start;
  }
}
