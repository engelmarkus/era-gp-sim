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

import QtQuick 2.0
import QtQuick.Controls 1.4
import "Components"
import QtQuick.Controls.Styles 1.4

Item {

    // Color definitions
    property color activeTabColor: "white"
    property color tabBarColor: Qt.rgba(236.0/255.0, 236.0/255.0, 236.0/255.0, 1.0)
    property color componentBackgroundColor: Qt.rgba(242.0/255.0, 242.0/255.0, 242.0/255.0, 1.0)
    property color titleColor: "#666666"
    property color titleColorHighlighted: "#444444"
    property color headerBackgroundColor: "#AAAAAA"
    // Measurement definitions
    property var headerHeightCollapsed: 5

    // Default value in the item
    property var usual
    property bool isExpanded: false

    onIsExpandedChanged: {
        if(isExpanded==true){
            headerFadeOut.stop()
            headerFadeIn.start()
        }else{
            headerFadeIn.stop()
            headerFadeOut.start()
        }
    }
    SequentialAnimation{
        id: headerFadeIn
        PauseAnimation { duration: 100 }
        PropertyAnimation { target: header; properties: "height"; to: "24"; duration: 100}
        PropertyAnimation { target: componentComboBox; properties: "visible"; to: true; duration: 0}
    }
    SequentialAnimation{
        id: headerFadeOut
        PauseAnimation { duration: 100 }
        PropertyAnimation { target: componentComboBox; properties: "visible"; to: false; duration: 0}
        PropertyAnimation { target: header; properties: "height"; to: headerHeightCollapsed; duration: 100}
    }

    // Header containing the componentComboBox
    Rectangle {
        id: header
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: headerHeightCollapsed
        color: headerBackgroundColor

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            propagateComposedEvents: true

            onHoveredChanged: {
                if(containsMouse || componentComboBox.hovered || componentComboBox.pressed){
                    isExpanded=true
                }else{
                    isExpanded=false
                }
            }
        }

        // ComboBox for choosing the current component
        ComboBox{
            id: componentComboBox
            anchors.top: parent.top
            visible: false
            width: parent.width
            height: 20
            model: ["Choose Component","Snapshots", "Output", /*"Editor",*/ "Register", "Memory" ]

            onCurrentIndexChanged:{
                if(currentIndex==0){
                    holder.change("nothing");
                } else if(currentIndex==1){
                    holder.change("snapshots");
                } else if(currentIndex==2){
                    holder.change("output");
                } else if(currentIndex==3){
                    holder.change("register");
                } else{
                    holder.change("memory");
                }
            }

            onPressedChanged: {
                if(pressed)
                    isExpanded=true
                else if(!hovered)
                    isExpanded=false
            }
            onHoveredChanged: {
                if(hovered)
                    isExpanded=true
                else if(!pressed)
                    isExpanded=false
            }

            style: ComboBoxStyle {
                label: Item {
                    // Component Icon
                    Image {
                        id: comboBoxIcon

                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.topMargin: 3
                        height: 0.75 * componentComboBox.height
                        fillMode: Image.PreserveAspectFit

                        source: "Component Icons/RegisterIcon.svg"

                        // Automatically select the corresponding icon when the selected
                        // index changed.
                        Component.onCompleted: {
                            componentComboBox.currentIndexChanged.connect(changeIcon)
                        }
                        function changeIcon() {
                            if (componentComboBox.currentIndex == 0) {
                                comboBoxIcon.source = "";
                            } else if (componentComboBox.currentIndex == 1) {
                                comboBoxIcon.source = "Component Icons/SnapshotIcon.svg";
                            } else if (componentComboBox.currentIndex == 2) {
                                comboBoxIcon.source = "Component Icons/OutputIcon.svg";
                            } else if (componentComboBox.currentIndex == 3) {
                                comboBoxIcon.source = "Component Icons/RegisterIcon.svg";
                            } else if (componentComboBox.currentIndex == 4) {
                                comboBoxIcon.source = "Component Icons/MemoryIcon.svg";
                            }
                        }
                    }
                    // Component Title
                    Text {
                        id: comboBoxLabel
                        text: control.editText
                        font.bold: true
                        color: titleColor
                        anchors.left: comboBoxIcon.right
                        anchors.leftMargin: 5
                        anchors.verticalCenter: comboBoxIcon.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    // Component Selection Indicator (small triangle)
                    Text {
                        id: comboBoxSelectionIndicator
                        anchors.left: comboBoxLabel.right
                        anchors.leftMargin: 2
                        anchors.verticalCenter: comboBoxLabel.verticalCenter
                        text: "\u25bc"
                        font.pointSize: 8
                        color: titleColor
                    }
                }
                // Remove ComboBox-background
                background: Rectangle {
                    visible: false
                }
            }
        }

    }

    // Holds the actual component
    ComponentHolder{
        id: holder
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        usual: parent.usual
    }
}
