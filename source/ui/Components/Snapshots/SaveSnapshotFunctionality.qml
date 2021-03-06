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

import Settings 1.0

Item {
  SingleSettingDialog {
    id: dialog
    property string snapshotName
    propertyName: 'overrideSnapshots'
    text: "Override snapshot <b>" + snapshotName + "</b>?"
    onDone: {
      if (answerWasYes) {
        save(snapshotName);
        didSave();
      } else {
        dontOverride();
      }
    }
  }

  signal dontOverride();
  signal didSave();

  // Takes a snapshot of the current project and saves it under the given name.
  //
  // \parm name The name of the snapshot to save.
  function save(name) {
    ui.saveSnapshot(tabView.currentProjectId(), name);
    didSave();
  }

  // Activates the snapshot saving functionality.
  //
  // \param name The name of the snapshot to save.
  // \param exists (optional) If passed, specifies whether the snapshot
  //        is known to already exist. If not passed at all, this is determined.
  function activate(name) {
    var exists = arguments[1];
    if (exists === undefined) {
      exists = guiProject.snapshotExists(name);
    }

    console.assert(exists !== undefined && exists !== null);
    if (!exists) {
      save(name);
      return;
    }

    if (dialog.alreadyAsked) {
      if (Settings.overrideSnapshots) {
        save(name);
      } else {
        dontOverride();
      }
    } else {
      dialog.snapshotName = name;
      dialog.open();
    }
  }
}
