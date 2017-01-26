/*
  * ERASIM Assembler Interpreter
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

#include "ui/input-text-model.hpp"
#include "core/conversions.hpp"

InputTextModel::InputTextModel(QQmlContext* context, MemoryAccess memoryAccess)
: QObject()
, _context(context)
, _start(0)
, _maximumLength(20)
, _memoryAccess(memoryAccess)
, _mode(Mode::ARRAY_BASED) {
  _context->setContextProperty("inputTextModel", this);
}

void InputTextModel::newText(QString text) {
  for (auto i : Utility::range<size_t>(0, text.length())) {
    if (_start + i > _memoryAccess.getMemorySize().get()) {
      // Too long
      break;
    }
    QChar qchar = text.at(i);
    if (qchar.isPrint()) {
      // else do nothing, was not an printable sign
      MemoryValue m = conversions::convert(qchar.unicode(), 32);
      _memoryAccess.putMemoryValueAt(_start + i, m);
    }
  }
}

void InputTextModel::newNumber(size_t number) {
  auto memoryValue = conversions::convert(number, 32);
  _memoryAccess.putMemoryValueAt(_start, memoryValue);
}

void InputTextModel::setStart(size_t start) {
  if (_memoryAccess.getMemorySize().get() >= start + _maximumLength) {
    _start = start;
  }
}

QString InputTextModel::getStart() {
  return QString::number(_start);
}

void InputTextModel::setMaximumLength(size_t maximumLength) {
  _maximumLength = maximumLength;
  emit maximumLengthChanged();
}

InputTextModel::size_t InputTextModel::getMaximumLength() {
  return _maximumLength;
}

void InputTextModel::setMode(int mode) {
  _mode = static_cast<Mode>(mode);
  emit modeChanged();
}

int InputTextModel::getMode() {
  return static_cast<int>(_mode);
}
