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

#include "snapshotmodel.hpp"
#include <QStringList>
#include <string>
#include <iostream>
#include <QQmlApplicationEngine>


SnapshotModel::SnapshotModel(QQmlContext *context, QObject *parent): QAbstractListModel(parent){
    this->context=context;
    context->setContextProperty("snapshotModel",  this);
}


void SnapshotModel::add(QString s){
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    list.append(s);
    list.sort();/*sorting the list*/
    endInsertRows();
}

bool SnapshotModel::removeRow(int row, const QModelIndex &parent){
    return removeRows(row, 1, parent);
}

bool SnapshotModel::removeRows(int row, int count, const QModelIndex &parent){
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), row, row+count-1);
    bool success=true;
    for(int i=0; i<count; i++){
        list.removeAt(row);
    }
    endRemoveRows();
    return success;
}

void SnapshotModel::deleteClicked(QByteArray i){
    std::string st=i.toStdString();
    int index=list.indexOf(QString::fromStdString(st));
    removeRow(index);
    std::cout<<"row "<<st+" "<<std::to_string(index)<<std::endl;
    QString liststring=list.join(", ");
    std::string stri=liststring.toStdString();
    std::cout<<stri<<std::endl;
}

void SnapshotModel::loadClicked(QByteArray i){
    std::string st=i.toStdString();
    std::cout<<"row "<<st<<" load"<<std::endl;
}

int SnapshotModel::rowCount(const QModelIndex &parent)const{
    Q_UNUSED(parent);
    return list.count();
}

/*returns data of a row*/
QVariant SnapshotModel::data(const QModelIndex &index, int role)const{
    if (index.row() < 0 || index.row() >= list.count()){
            return QVariant();
    }

    QString s =list.at(index.row());
    if (role == NameRole){
            return s;
    }
    return QVariant();

}

void SnapshotModel::addList(std::string names[]){
    int length=0;
    QStringList newNames;
    while(!names[length].empty()){
        length++;
    }
    for(int i=0; i<length; i++){
        QString s=QString::fromStdString(names[i]);
        newNames.append(s);
    }
    emit addToList(newNames);
}

void SnapshotModel::addListQML(QStringList names){
//    int length=0;
//    while(!names[length].empty()){
//        length++;
//    }
//    for(int i=0; i<length; i++){
//        add(names[i]);
//    }
    int length=names.count();
    for(int i=0; i<length; +i++){
        add(names.at(i));
    }

}

QHash<int, QByteArray> SnapshotModel::roleNames()const{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}





