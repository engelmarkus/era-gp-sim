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

#include "ui/memory-component-presenter.hpp"
#include <iostream>

#include "common/assert.hpp"
#include "common/string-conversions.hpp"
#include "core/memory-value.hpp"

MemoryComponentPresenter::MemoryComponentPresenter(MemoryAccess access,
                                                   MemoryManager manager,
                                                   QQmlContext *projectContext,
                                                   QObject *parent)
: QAbstractListModel(parent)
, _memoryAccess(access)
, _memoryManager(manager)
, _memorySize(access.getMemorySize().get()) {
  projectContext->setContextProperty("memoryModel", this);
}


MemoryComponentPresenter::~MemoryComponentPresenter() {
}


void MemoryComponentPresenter::onMemoryChanged(std::size_t address,
                                               std::size_t length) {
  emit dataChanged(this->index(address), this->index(address + length - 1));
}


void MemoryComponentPresenter::setValue(int address, QString number) {
  _memoryAccess.putMemoryValueAt(
      address,
      *StringConversions::hexStringToMemoryValue(number.toStdString(), 8));
}


void MemoryComponentPresenter::setContextInformation(int addressStart,
                                                     int length,
                                                     int identifier) {
  // TODO
}


int MemoryComponentPresenter::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return _memorySize;
}


QVariant
MemoryComponentPresenter::data(const QModelIndex &index, int role) const {
  // check boundaries
  assert::that(index.isValid());

  // get role as a string because there is more information in it
  QString role_string = MemoryComponentPresenter::roleNames().value(role);

  int number_of_bits = 8;
  if (role_string.endsWith("16")) number_of_bits = 16;
  if (role_string.endsWith("32")) number_of_bits = 32;

  if (role_string.startsWith("address")) {
    // format index as hex value and return it
    return QString("%1")
        .arg(index.row() * number_of_bits / 8, 4, 16, QLatin1Char('0'))
        .toUpper()
        .prepend("0x");
  }

  if (role == InfoRole) {
    // TODO fetch value from core
    return QString("");
  }

  MemoryValue memory_cell =
      _memoryAccess.getMemoryValueAt(index.row(), number_of_bits / 8).get();
  std::string stringvalue;
  if (role_string.startsWith("bin"))
    stringvalue = StringConversions::toBinString(memory_cell);
  // else if (role_string.startsWith("oct"))
  //  stringvalue = StringConversions::toOctString(memory_cell);
  else if (role_string.startsWith("hex"))
    stringvalue = StringConversions::toHexString(memory_cell);
  else if (role_string.startsWith("decs"))
    stringvalue = StringConversions::toSignedDecString(memory_cell);
  else if (role_string.startsWith("dec"))
    stringvalue = StringConversions::toUnsignedDecString(memory_cell);
  else
    stringvalue = "unknown format";

  return QString::fromStdString(stringvalue);

  // unknown column role, return empty value
  // return QVariant();
}


QHash<int, QByteArray> MemoryComponentPresenter::roleNames() const {
  // connect TableColumns in View with columns in this model
  QHash<int, QByteArray> roles;
  roles[AddressRole8] = "address8";
  roles[AddressRole16] = "address16";
  roles[AddressRole32] = "address32";
  roles[ValueRoleBin8] = "bin8";
  roles[ValueRoleBin16] = "bin16";
  roles[ValueRoleBin32] = "bin32";
  roles[ValueRoleOct8] = "oct8";
  roles[ValueRoleOct16] = "oct16";
  roles[ValueRoleOct32] = "oct32";
  roles[ValueRoleHex8] = "hex8";
  roles[ValueRoleHex16] = "hex16";
  roles[ValueRoleHex32] = "hex32";
  roles[ValueRoleDec8] = "dec8";
  roles[ValueRoleDec16] = "dec16";
  roles[ValueRoleDec32] = "dec32";
  roles[ValueRoleDecS8] = "decs8";
  roles[ValueRoleDecS16] = "decs16";
  roles[ValueRoleDecS32] = "decs32";
  roles[InfoRole] = "info";
  return roles;
}
