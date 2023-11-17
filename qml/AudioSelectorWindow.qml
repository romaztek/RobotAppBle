import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    anchors.fill: parent
    anchors.margins: 5

    function setVolumeSliderFocus() {
        volumeSlider.focus = true
    }

    ButtonGroup {
        id: audioButtonsGroup
    }

    GridLayout {
        id: audioButtonsGrid
        width: parent.width
        anchors.margins: 5
        columns: 3

        MyRadioButton {
            id: audioButton1
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            buttonGroup: audioButtonsGroup

            text: qsTr("1")

            onClicked: {
                btController.sendMessage(btCommands.audio1Command(), current_head)
            }
        }
        MyRadioButton {
            id: audioButton2
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            buttonGroup: audioButtonsGroup

            text: qsTr("2")

            onClicked: {
                btController.sendMessage(btCommands.audio2Command(), current_head)
            }
        }
        MyRadioButton {
            id: audioButton3
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            buttonGroup: audioButtonsGroup

            text: qsTr("3")

            onClicked: {
                btController.sendMessage(btCommands.audio3Command(), current_head)
            }
        }
        MyRadioButton {
            id: audioButton4
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            buttonGroup: audioButtonsGroup

            text: qsTr("4")

            onClicked: {
                btController.sendMessage(btCommands.audio4Command(), current_head)
            }
        }
        MyRadioButton {
            id: audioButton5
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            buttonGroup: audioButtonsGroup

            text: qsTr("5")

            onClicked: {
                btController.sendMessage(btCommands.audio5Command(), current_head)
            }
        }
        MyRadioButton {
            id: audioButton6
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            buttonGroup: audioButtonsGroup

            text: qsTr("6")

            onClicked: {
                btController.sendMessage(btCommands.audio6Command(), current_head)
            }
        }
        Rectangle {
            id: audioButtonStop
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
                    for(var i = 0; i < audioButtonsGrid.children.length; i++) {
                        if(audioButtonsGrid.children[i].toString().includes("MyRadioButton")) {
                            audioButtonsGrid.children[i].checked = false
                        }
                    }

                    btController.sendMessage(btCommands.audioStopCommand(), current_head)
                }
            }
        }
    }

    Slider {
        id: volumeSlider
        width: audioButtonsGrid.width
        anchors.top: audioButtonsGrid.bottom
        anchors.topMargin: 5
        value: 5
        from: 1
        to: 5
        stepSize: 1
        focus: true
        onValueChanged: {
            switch (value) {
            case 1:
                btController.sendMessage(btCommands.audioVolume1Command(), current_head)
                break
            case 2:
                btController.sendMessage(btCommands.audioVolume2Command(), current_head)
                break
            case 3:
                btController.sendMessage(btCommands.audioVolume3Command(), current_head)
                break
            case 4:
                btController.sendMessage(btCommands.audioVolume4Command(), current_head)
                break
            case 5:
                btController.sendMessage(btCommands.audioVolume5Command(), current_head)
                break
            default:
                break
            }
        }
        Keys.onPressed: {
            if (event.key === Qt.Key_VolumeUp) {
                volumeSlider.increase()
                event.accepted = true
            } else if (event.key === Qt.Key_VolumeDown) {
                volumeSlider.decrease()
                event.accepted = true
            }
        }
    }
    Label {
        anchors.top: volumeSlider.bottom
        anchors.topMargin: 5
        text: volumeSlider.value
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
