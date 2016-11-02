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

#ifndef ERAGPSIM_UI_MEMORY_COMPONENT_PRESENTER_HPP
#define ERAGPSIM_UI_MEMORY_COMPONENT_PRESENTER_HPP

#include <QAbstractListModel>
#include <QHash>
#include <QModelIndex>
#include <QObject>
#include <QQmlContext>
#include <QVariant>

#include "core/memory-access.hpp"
#include "core/memory-manager.hpp"

class MemoryComponentPresenter : public QAbstractListModel {
  Q_OBJECT

 public:
  explicit MemoryComponentPresenter(MemoryAccess access,
                                    MemoryManager manager,
                                    QQmlContext *projectContext,
                                    QObject *parent = 0);
  ~MemoryComponentPresenter();

  /**
   * converts a hexademcimal representation of a string into a memory value
   * and saves it to the internal memory object
   *
   * /param address the address of the cell to be updated
   * /param newvalue the new value for the memory cell in hexadecimal
   * representation
   *
   */
  Q_INVOKABLE void setValue(int address, QString newvalue);

  /**
   * sets the context information for memory cells (NOT IMPLEMENTED YET)
   *
   * /param addressStart the starting address of the memory cell the context
   * information is related to
   * /param length the number of memory cells the context information is related
   * to
   * /param identifier the unique identifier for this context information (for
   * further updates)
   *
   */
  void setContextInformation(int addressStart, int length, int identifier);

 private:
  /**
   * Returns the data stored under the given role for the item referred to by
   * the index.
   * Inherited from QAbstractListModel
   *
   * /param index the index where the data should be written to
   * /param role one of several DisplayRoles for this column
   * /return returns the QVariant that is displayed in the view
   *
   */
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

  /**
   * returns the number of rows in this table
   * inherited from QAbstractListModel
   *
   * /param parent pointer to the logical data parent
   * /return returns the length of the table
   *
   */
  int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

  /**
   * returns the translation between roleNames in QML and the internal index
   * representation inherited from QAbstractListModel
   *
   * /return returns a QHash with the connection between the internal index
   * representation and the name of the role in QML
   *
   */
  QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

  /** holds a MemoryAccess for accessing the memory */
  MemoryAccess _memoryAccess;

  /** holds a MemoryManager that handles the registration for callback functions
   */
  MemoryManager _memoryManager;


  /** saves the size of the memory, as calling MemoryAccess::getMemorySize() in
   * rowCount causes a deadlock. */
  std::size_t _memorySize;

  /** enumeration of all roles of the columns */
  enum ColumnRoles {
    AddressRole = Qt::UserRole,// avoid collisions with predefined roles
    ValueRole,
    InfoRole
  };

 public slots:
  /**
   * callback function for the core memory. Is beeing called when something in
   * the
   * memory changes
   *
   * /param address the address of the first cell with a new value
   * /param length the number of cells that were changed
   *
   */
  void onMemoryChanged(const std::size_t address, const std::size_t length);
};

#endif /* ERAGPSIM_UI_MEMORY_COMPONENT_PRESENTER_HPP */