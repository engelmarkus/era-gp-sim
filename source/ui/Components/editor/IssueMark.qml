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
import QtQuick.Controls 1.5
import QtQuick.Dialogs 1.2
import "../Common"
import "../Common/TextUtilities.js" as TextUtilities

// Each line wich contains any issue (error, warning or information) is marked by
// an issueMark. An issueMark is the container of one or more issueItems.
Item {
    id: issueMark

    // The issueItems this issueMark contains. See issueMark for more information on this.
    property var issueItems: []

    // The linenumber this issue mark belongs to.
    property var lineNumber: 0

    // Issue line highlight should span whole editor width and the line the issue belongs to.
    height: textArea.cursorRectangle.height
    width: scrollView.width

    // If the issue mark is expanded, it shows the issue icon inside the errorBar as well
    // as the issueLineHighlight along with its issueItems (containing issue messages).
    // If collapsed, it only shows the issue icon inside the errorBar.
    property var expanded: true
    // An issue mark is a container for one or more issueItems which can have varying issueTypes.
    // Therefore the container always displays the most important issueType among all its
    // child-issueItems (Error > Warning > Information).
    property var dominantIssueType: ""

    // Color definitions
    property var errorColorSolid: Qt.rgba(1.0, 185.0/255.0, 152.0/255.0, 1.0)
    property var errorColorLight: Qt.rgba(1.0, 80.0/255.0, 0.0, 0.14)
    property var warningColorSolid: Qt.rgba(1.0, 227.0/255.0, 157.0/255.0, 1.0)
    property var warningColorLight: Qt.rgba(1.0, 185.0/255.0, 10.0/255.0, 0.14)
    property var informationColorSolid: Qt.rgba(153.0/255.0, 200.0/255.0, 255.0/255.0, 1.0)
    property var informationColorLight: Qt.rgba(0.0, 117.0/255.0, 255.0/255.0, 0.14)

    // The issue's Icon inside the error bar.
    Image {
        id: issueIcon

        width: parent.height-3
        height: parent.height-3
        x: (errorBar - width) / 2
        anchors.verticalCenter: issueLineHighlight.verticalCenter
        source: {
            switch (dominantIssueType) {
            case "Error":
                return "Issue Icons/Error Icon.png";
            case "Warning":
                return "Issue Icons/Warning Icon.png";
            case "Information":
                return "Issue Icons/Information Icon.png";
            default:
                return "Issue Icons/Error Icon.png";
            }
        }

        // Use issueIcon as button for expanding/collapsing issueMark.
        MouseArea {
            anchors.fill: parent
            preventStealing: true
            onClicked: {
                expanded = !expanded;
            }
        }
    }

    // Highlights the line the issue belongs to.
    Rectangle {
        id: issueLineHighlight

        color: errorColorLight
        visible: expanded

        anchors.fill: parent

        // An issueItem marks an issue (error, warning, information) inside the corresponding line.
        Component {
            id: issueItemComponent

            Rectangle {
                id: issueMarkTextBackground

                // The issue message to display.
                property var issueMessage: "";
                // The type of issue (error, warning, information).
                property var issueType: "";
                // Margin between issue item icon, issue text and right edge.
                property var _textMargin: 4

                color: {
                    switch (issueType) {
                    case "Error":
                        return errorColorSolid;
                    case "Warning":
                        return warningColorSolid;
                    case "Information":
                        return informationColorSolid;
                    default:
                        return errorColorSolid;
                    }
                }

                // TextMetrics do not take line breaks into account for their width. Therefore the
                // issueMarkTextBackground should be just as wide as the issueTextMetrics' width but should
                // always fit into the editor (
                width: Math.min(issueTextMetrics.width, issueText.width) + 3 * _textMargin + issueItemIcon.width
                height: Math.max(issueText.height, issueLineHighlight.height)

                // Each issueItem displays an icon corresponding to its issueType.
                Image {
                    id: issueItemIcon
                    width: issueIcon.width
                    height: issueIcon.height
                    anchors.left: parent.left
                    anchors.leftMargin: _textMargin
                    anchors.verticalCenter: parent.verticalCenter
                    source: {
                        switch (issueType) {
                        case "Error":
                            return "Issue Icons/Error Icon.png";
                        case "Warning":
                            return "Issue Icons/Warning Icon.png";
                        case "Information":
                            return "Issue Icons/Information Icon.png";
                        default:
                            return "Issue Icons/Error Icon.png";
                        }
                    }
                }

                // Required for finding out whether issue text overlaps line text.
                TextMetrics {
                    id: issueTextMetrics
                    text: issueText.text
                    font: issueText.font
                }

                // Displays the issueMessage (error message, warning message, information message).
                Text {
                    id: issueText
                    anchors.right: parent.right
                    anchors.rightMargin: _textMargin
                    anchors.verticalCenter: parent.verticalCenter
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    width: scrollView.width - sidebar.width - issueItemIcon.width - 3*_textMargin

                    font.pixelSize: 10
                    text: issueMessage
                    horizontalAlignment: Text.AlignRight

                }
            }

        }

        // Destroy issues when signal is sent.
        Connections {
            target: editor
            onDeleteErrors: {
                issueMark.destroy();
            }
        }

        // Adds a new issue item to the current issueMark.
        function addIssueItem(message, lineNumber, issueType) {
            var newIssueItem = issueItemComponent.createObject();
            newIssueItem.parent = issueLineHighlight;
            newIssueItem.issueMessage = message;
            newIssueItem.issueType = issueType;

            newIssueItem.anchors.right = issueLineHighlight.right;

            if (issueMark.issueItems.length === 0) {
                // Position first item.
                issueMark.issueItems.push(newIssueItem);
                _offsetFirstIssueItemIfNecessary()
            } else {
                // Anchor the new item to its neighboring item above.
                newIssueItem.anchors.top = issueMark.issueItems[issueMark.issueItems.length-1].bottom;
                issueMark.issueItems.push(newIssueItem);
            }

        }

        // Check if the first issueItem needs to be offset by one line if it would
        // otherwise ovrlap with the line text after the editor's width has changed.
        onWidthChanged: {
            _offsetFirstIssueItemIfNecessary();
        }

        // Checks if the first issueText would overlap the line text and offsets it by one line
        // if necessary.
        function _offsetFirstIssueItemIfNecessary() {
            if (issueMark.issueItems.length == 0 ) return;
            // Check if the first issueText would overlap the line text.
            var lineEndX = textArea.positionToRectangle(TextUtilities.getLineEndForLine(textArea.text, lineNumber)).x;
            var errorLeftX = textArea.width - issueMark.issueItems[0].width;
            var textToErrorDistance = errorLeftX - lineEndX;
            // If it would overlap, offset it by one line.
            if (textToErrorDistance < 10) {
                issueItems[0].y = issueLineHighlight.height;
            } else { // Oterhwise, position it at the issueMark's first line.
                issueItems[0].y = 0;
            }
        }
    }

    // Adds a new issue item to the current issueMark.
    function addIssueItem(message, lineNumber, issueType) {
        issueLineHighlight.addIssueItem(message, issueType);
    }
}