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

#ifndef SNAPSHOTMODEL_H
#define SNAPSHOTMODEL_H

#include <string>
#include <QObject>
#include <QStringList>
#include <QAbstractListModel>
#include <QQmlContext>

class SnapshotModel : public QAbstractListModel{
    Q_OBJECT
public:
    enum Roles {
            NameRole=Qt::UserRole+1
     };

    SnapshotModel(QQmlContext* context, QObject *parent = 0);

    bool removeRow(int row, const QModelIndex &parent = QModelIndex());

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());


    /*
     * \brief Used for the qml-List, the name can only be used once
     * \param name of the entry
    */
    Q_INVOKABLE void deleteClicked(QByteArray i);

    /*
     * \brief Used for the load-button in the qml-list
     * \param name of the entry, which should be load
    */
    Q_INVOKABLE void loadClicked(QByteArray i);


    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    /*
     * \brief can be used to add an array of names to the snapshotmodel
     * \parem the array
     */
    void addList(std::string names[]);

    Q_INVOKABLE void addListQML(QStringList names);



protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QStringList list;
    QQmlContext* context;
    void add(QString s);

signals:
    void addToList(QStringList nameList);
};


#endif // SNAPSHOTMODEL_H
