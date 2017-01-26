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

import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

import Theme 1.0

SplitView{
  property var usual1
  property var usual2

  /*heigth of the first component*/
  property int quotient
  property int faktor

  SystemPalette {
    id: systemPalette
  }

  // height and witdth have to be greater than 1 to achieve correct behaviour on
  // devices with touchscreen
  handleDelegate: Rectangle {
    width: Theme.splitview.handleWidth
    height: Theme.splitview.handleHeight
    color: Theme.splitview.handleColor
  }

  orientation: Qt.Vertical
  SplitviewItem{
    id: item1
    Layout.minimumHeight: 25
    height: parent.height/2
    usual: parent.usual1
  }

  SplitviewItem{
    id: item2
    height: parent.height/2
    Layout.minimumHeight: 10
    usual: parent.usual2
    onUsualChanged: {
      //console.info("SplitviewItem: "+usual+parent.usual2);

    }
  }


  onUsual1Changed: {
    item1.usual=usual1;
  }

  onUsual2Changed: {
    item2.usual=usual2;
  }

  /*Sets heigth at the beginning*/
  onHeightChanged: {
    item1.height=faktor*(height/quotient)-5;
    item2.height=height-item1.height-5;
  }

}
