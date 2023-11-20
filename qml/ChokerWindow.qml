import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    anchors.fill: parent
    anchors.margins: 5

    ButtonGroup {
        id: chokerButtonsGroup
    }

    GridLayout {
        id: chokerButtonsGrid
        width: parent.width
        anchors.margins: 5
        columns: 3

        MyRadioButton {
            id: chokerButton1
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            buttonGroup: chokerButtonsGroup

            text: qsTr("1")

            onClicked: {
                btController.sendMessage(btCommands.chocker1Command(), current_head)
            }
        }

        MyRadioButton {
            id: chokerButton2
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            buttonGroup: chokerButtonsGroup

            text: qsTr("2")

            onClicked: {
                btController.sendMessage(btCommands.chocker2Command(), current_head)
            }
        }

        MyRadioButton {
            id: chokerButton3
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            buttonGroup: chokerButtonsGroup

            text: qsTr("3")

            onClicked: {
                btController.sendMessage(btCommands.chocker3Command(), current_head)
            }
        }

        MyRadioButton {
            id: chokerButton4
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            buttonGroup: chokerButtonsGroup

            text: qsTr("4")

            onClicked: {
                btController.sendMessage(btCommands.chocker4Command(), current_head)
            }
        }

        Rectangle {
            id: chokerButtonOff
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            border.width: 2
            radius: itemRadius
            color: selected ? "lime" : defaultColor
            property bool selected: false
            Label {
                text: qsTr("Stop")
                elide: Text.ElideMiddle
                wrapMode: Text.WordWrap
                x: 5
                verticalAlignment: Qt.AlignVCenter
                height: parent.height
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    for(var i = 0; i < chokerButtonsGrid.children.length; i++) {
                        if(chokerButtonsGrid.children[i].toString().includes("MyRadioButton")) {
                            chokerButtonsGrid.children[i].checked = false
                        }
                    }

                    btController.sendMessage(btCommands.chockerOffCommand(), current_head)
                }
            }
        }

    }
}
