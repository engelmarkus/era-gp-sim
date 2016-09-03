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

#include "menubarmodel.hpp"

#include <string>
#include <iostream>

MenubarModel::MenubarModel(QQmlContext *context): QObject(){
    this->context=context;
    context->setContextProperty("menubarModel",  this);
}

void MenubarModel::open(QByteArray name){
    std::cout<<"C++ open"<<std::endl;
    std::string stdName=name.toStdString();
    //QProject open
}

void MenubarModel::save(){
    std::cout<<"C++ save"<<std::endl;
    //QProject save
}

void MenubarModel::saveAs(QByteArray name){
    std::cout<<"C++ saveAs"<<std::endl;
    std::string stdName=name.toStdString();
    //QProject saveAs
}

void MenubarModel::newTab(QByteArray arch, QByteArray name){
     std::string stdArch=arch.toStdString();
     std::string stdName=name.toStdString();
     //Core New
}
