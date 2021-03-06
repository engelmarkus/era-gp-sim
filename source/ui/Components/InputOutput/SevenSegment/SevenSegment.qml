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

/*
  A 7-segment control for displaying numbers:
   _   _
  |_| |_|
  |_| |_|

*/

import QtQuick 2.6
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Theme 1.0
import "../"

Rectangle {
  id: sevenSegmentOutputItem

  // Index of output item relative to all available output outems.
  // Required to identify this output item when communicating with
  // the model.
  property var outputItemIndex: 1

  signal settingsButtonPressed()

  // Color definitions
  property var digitsInactiveColor: Theme.output.sevenSegment.digit.inactive.background
  property var digitsActiveColor: Theme.output.sevenSegment.digit.active.background
  property var digitsTextColor: Theme.output.sevenSegment.digit.color

  color: Theme.output.sevenSegment.background

  // Connect the output item to signals that the model might send.
  Connections {
    target: outputComponent
    // Send when the memory changes (at any address).
    onMemoryChanged: {
      var _baseAddress = outputComponent.getOutputItem(outputItemIndex)["baseAddress"];
      var numberOfDigits = outputComponent.getOutputItem(outputItemIndex)["numberOfDigits"];
      // Check if the memory address that was changed (at least partly) belongs to
      // the output item's source space.
      if ((address+length) >= _baseAddress && (address <= (_baseAddress+(numberOfDigits)))) {
        updateContent(_baseAddress);
      }
    }
    // Sent when any item's settings where updated.
    onOutputItemSettingsChanged: {
      updateContent(outputComponent.getOutputItem(outputItemIndex)["baseAddress"]);
      sevenSegmentSettingsWindow.updateSettings();
    }
  }

  // Update the output item's content (there may already be some initial values in memory).
  Component.onCompleted: {
    updateContent(outputComponent.getOutputItem(outputItemIndex)["baseAddress"]);
  }

  // Called from outside by the output tab view to signal that the settings button for the current
  // output item was pressed.
  onSettingsButtonPressed: {
    sevenSegmentSettingsWindow.show();
  }

  ListModel {
    id: sevenSegmentDigitsModel
  }

  // ListView for seven segment digits.
  ListView {
    id: digitsListView

    anchors.centerIn: parent
    height: parent.height - (2 * Theme.output.sevenSegment.verticalMargin)
    spacing: Theme.output.sevenSegment.spacing
    width: (height / 2) * count + (spacing) * (count - 1)

    orientation: ListView.Horizontal
    interactive: false

    model: sevenSegmentDigitsModel

    delegate: sevenSegmentDigit
  }

  // Delegate for the ListView displaying each digit.
  Component {
    id: sevenSegmentDigit

    SevenSegmentDigit {
      height: parent.height
      width: height / 2
      thickness: height / 10

      color: "#00000000"  // Background Color
      digitActiveColor: activeColor
      digitInactiveColor: digitsInactiveColor
      textColor: digitsTextColor

      segment0Activated: segment0
      segment1Activated: segment1
      segment2Activated: segment2
      segment3Activated: segment3
      segment4Activated: segment4
      segment5Activated: segment5
      segment6Activated: segment6

      onSegmentPressed: {
        var segmentIdentifier = "segment" + segmentIndex;
        sevenSegmentDigitsModel.setProperty(index, segmentIdentifier, !sevenSegmentDigitsModel.get(index)[segmentIdentifier]);
        var memoryContent = [];
        for (var digitIndex = (sevenSegmentDigitsModel.count -1); digitIndex >= 0; --digitIndex) {
          for (var currentSegmentIndex = 0; currentSegmentIndex < 7; ++currentSegmentIndex) {
            memoryContent.push(sevenSegmentDigitsModel.get(digitIndex)["segment" + currentSegmentIndex]);
          }
          memoryContent.push(false);  // Fill one byte.
        }
        var _baseAddress = outputComponent.getOutputItem(outputItemIndex)["baseAddress"];
        outputComponent.putMemoryValue(_baseAddress, memoryContent);
      }

      MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
          // Show change-color-dialog on right-click.
          var newObject = colorChooserComponent.createObject(sevenSegmentDigit);
        }

        Component {
          id: colorChooserComponent
          ColorChooser {
            id: colorDialog
            currentColor: sevenSegmentDigitsModel.get(index).activeColor
            onAccepted: {
              sevenSegmentDigitsModel.setProperty(index, "activeColor", colorDialog.color.toString());
              colorDialog.close();
            }
          }
        }
      }
    }
  }


  // Updates the content of the output model depending on the value in memory.
  function updateContent(_baseAddress) {
    _updateSevenSegmentDigitsModel();

    var content = outputComponent.getMemoryContent(_baseAddress, sevenSegmentDigitsModel.count);
    for (var digitIndex = 0; digitIndex < sevenSegmentDigitsModel.count; ++digitIndex) {
      // Iterate memory bytewise from right to left, so the rightmost digit represents the first byte in memory.
      var digitInContentIndex = sevenSegmentDigitsModel.count -1 - digitIndex;
      for (var currentSegmentIndex = 0; currentSegmentIndex < 7; ++currentSegmentIndex) {
        var segmentIdentifier = "segment" + currentSegmentIndex;
        var bitValue = content[digitInContentIndex*8+currentSegmentIndex];
        sevenSegmentDigitsModel.setProperty(digitIndex, segmentIdentifier, bitValue);
      }
    }
  }

  // Updates the sevenSegmentDigitsModel to correspond to the output item's settings values.
  function _updateSevenSegmentDigitsModel() {
    var numberOfDigits = outputComponent.getOutputItem(outputItemIndex)["numberOfDigits"];
    if (numberOfDigits < 0) {
      numberOfDigits = 0;
    }
    while (sevenSegmentDigitsModel.count < numberOfDigits) {
      sevenSegmentDigitsModel.insert(0, { segment0: false,
                                       segment1: false,
                                       segment2: false,
                                       segment3: false,
                                       segment4: false,
                                       segment5: false,
                                       segment6: false,
                                       activeColor: digitsActiveColor});
    }
    while (sevenSegmentDigitsModel.count > numberOfDigits) {
      sevenSegmentDigitsModel.remove(0, 1);
    }
  }

  SevenSegmentSettingsWindow {
    id: sevenSegmentSettingsWindow
  }
}
