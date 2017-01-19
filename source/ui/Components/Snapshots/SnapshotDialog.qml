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

import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import Theme 1.0

Dialog {
  id: dialog
  title: "Save Snapshot"

  contentItem: Rectangle {
    implicitHeight: Theme.snapshots.creationDialog.height
    implicitWidth: Theme.snapshots.creationDialog.width

    TextField {
      id: name
      focus: true
      placeholderText: "Give your snapshot a name"

      anchors {
        top: parent.top
        topMargin: Theme.snapshots.creationDialog.marginTop
        left: parent.left
        leftMargin: Theme.snapshots.creationDialog.marginLeft
        right: parent.right
        rightMargin: Theme.snapshots.creationDialog.marginRight
      }

      style: TextFieldStyle {
        textColor: Theme.snapshots.creationDialog.input.color
        font.pixelSize: Theme.snapshots.creationDialog.input.fontSize
        background: Rectangle {
          color: Theme.snapshots.creationDialog.input.background
          border.color: Theme.snapshots.creationDialog.input.border.color
          border.width: Theme.snapshots.creationDialog.input.border.width
          radius: Theme.snapshots.creationDialog.input.radius
        }
      }
      Keys.onPressed: {
        if (event.key === Qt.Key_Return && text.length > 0) {
          save.clicked();
        }
      }
    }

    Button {
      id: save
      enabled: name.text.length > 0
      text: 'Save'
      anchors {
        top: cancel.top
        bottom: cancel.bottom
        right: cancel.left
      }
      onClicked: {
        ui.saveSnapshot(tabView.getCurrentProjectId(), name.text);
        name.text = ''
        dialog.close()
      }
    }

    Button {
      id: cancel
      text: 'Cancel'
      action: Action {
        shortcut: 'Esc'
        onTriggered: dialog.close()
      }
      anchors {
        right: parent.right
        rightMargin: Theme.snapshots.creationDialog.button.marginRight
        top: name.bottom
        topMargin: Theme.snapshots.creationDialog.button.marginTop
      }
    }
  }
}
