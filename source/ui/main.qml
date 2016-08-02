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
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "Components"
import "Components/Menubar"
import "Components/Toolbar"

ApplicationWindow {
    id: window
    visible: true
    // Set default window size
    width: 750
    height: 500

    property color activeTabColor: "white"
    property color tabBarColor: Qt.rgba(236.0/255.0, 236.0/255.0, 236.0/255.0, 1.0)
    property color componentBackgroundColor: Qt.rgba(242.0/255.0, 242.0/255.0, 242.0/255.0, 1.0)
    property color titleColor: "#666666"
    property color titleColorHighlighted: "444444"
    style: ApplicationWindowStyle {
        // The windows background color is visible through a small gap between the
        // tabView's tabBar and the split view. It's set to white to align with
        // the color of an active tab. Gives the impression of stacked record cards.
        background: Rectangle { color: activeTabColor }
    }

    menuBar: Menubar{
        component: tabs
        tabView: tabView
    }
    toolBar: ToolbarMainWindow{
        id: toolbar
    }

    TabView{
        id: tabView
        anchors.fill: parent

        property int counter: 1
        function increase(){
            counter=counter+1;
        }

        // Custom TabViewStyle
        style: TabViewStyle {
            property int tabHeight: 25

            // Overlap between tabs and the components below set to leave a small space
            // which gives the impression of stacked record cards.
            frameOverlap: -3

            // Sets the style of the blank area next to the tabs
            tabBar : Rectangle {
                color: tabBarColor
            }
            // Sets the style of the background of an open tab window
            frame : Rectangle {
                color: componentBackgroundColor
                // Creates a black border between tab bar and the active tab window
                Rectangle {
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent
                    width: parent.width
                    height: 1
                    color: "#9E9E9E"
                }
            }
            // Adds a plus button to the left corner
            leftCorner: Rectangle {
                implicitWidth: tabHeight
                implicitHeight: tabHeight
                color: tabBarColor
                Button {
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: -2
                    style: ButtonStyle {
                        background: Rectangle {
                            color: "transparent"
                        }
                        label: Text {
                            text: "\u002B"
                            anchors.centerIn: parent
                            color: control.pressed ? "#444444" : "#666666"
                            font.bold: true
                            font.pointSize: 18
                        }
                    }
                    onClicked: {
                        tabView.addTab("TestFile.asm", tabs);
                    }
                }
            }

            // Custom style for tabs
            tab: Rectangle {
                id: tabStyle
                color: styleData.selected ? activeTabColor : Qt.rgba(212.0/255.0, 212.0/255.0, 212.0/255.0, 1.0)
                implicitWidth: Math.max(tabTitle.width + tabCloseButton.width + 8, 80)
                implicitHeight: tabHeight
                Item {
                    anchors.centerIn: parent
                    width: tabTitle.width + tabCloseButton.width
                    // Tab title
                    Text {
                        id: tabTitle
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        text: styleData.title
                        color: titleColor
                    }
                    // Tab close Button
                    Button {
                        id: tabCloseButton
                        anchors.left: tabTitle.right
                        anchors.verticalCenter: tabTitle.verticalCenter
                        height: 16
                        style: ButtonStyle {
                            background: Rectangle {
                                implicitWidth: 16
                                implicitHeight: 16
                                color: "transparent"
                                Text {
                                    text: "\u00d7" ; anchors.centerIn: parent ;
                                    color: control.hovered ? titleColorHighlighted : titleColor
                                    font.bold: true
                                }
                            }
                        }
                        onClicked: {
                            // TODO: Remove tab
                        }
                    }
                }
            }
        }

    }

    /*Componetn which shuld be hold int the tabView*/
    Component{
        id: tabs
        Splitview{
            anchors.fill: parent
        }
    }
}
